"""
Internal support for writing to arrays frozen by freeze-on-view.

See the ``freeze_on_view`` array flag: while a view of such an array is alive,
both the base and its views are read-only.  `allow_view_writes` lifts that
restriction for a bounded region of code.
"""
from contextlib import contextmanager

from ._multiarray_umath import _set_allow_view_writes

__all__ = []


@contextmanager
def allow_view_writes():
    """
    Context manager that temporarily allows writing to frozen views.

    This is an internal helper.  Within the context, arrays that are read-only
    only because of freeze-on-view -- a frozen base with a live view, or a view
    of such a base -- may be written to.  Arrays that are read-only for any
    other reason (``arr.flags.writeable = False``, a broadcast result, a
    read-only buffer) are unaffected and still raise.

    The exemption applies to the calling thread only, so concurrent writes from
    other threads remain protected.  Nesting is supported: the previous setting
    is restored on exit.

    Notes
    -----
    This bypasses the invariant that freeze-on-view exists to enforce, namely
    that a base cannot change while something observes it through a view.  Use
    it only where the write is known not to be observable, and keep the context
    as small as possible.

    Examples
    --------
    >>> import numpy as np
    >>> from numpy._core import allow_view_writes
    >>> a = np.arange(5)
    >>> a.flags.freeze_on_view = True
    >>> v = a[1:]           # freezes ``a`` and ``v``
    >>> a[0] = 10
    Traceback (most recent call last):
        ...
    ValueError: assignment destination has freeze_on_view enabled and cannot \
be written to while a view of it exists
    >>> with allow_view_writes():
    ...     a[0] = 10
    >>> a[0]
    10

    """
    old = _set_allow_view_writes(True)
    try:
        yield
    finally:
        _set_allow_view_writes(old)
