.. _upgrading:

Upgrading :literal:`metricq`
============================

:literal:`metricq` follows the `semver` versioning scheme,
so releases of new major versions contain breaking changes.
These are listed below, together with suggested steps to
take when upgrading.

`1.x` → `2.0`
-------------

Changed required Python version to 3.8
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Python 3.7 has had issues with TLS/SSL encrypted connections,
therefore the minimum required version was changed to *3.8*.
Make sure, a matching Python version is available on the system.


Location of exception classes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. py:currentmodule:: metricq.exceptions

All exception classes have moved into the module :mod:`metricq.exceptions`,
documented :ref:`here <exceptions>`.
If you are importing exceptions from other submodules directly, your code might
now fail with an :exc:`python:ImportError`.

:code:`AgentStoppedError`, :code:`ConnectFailedError`, :code:`ReceivedSignalError`, :code:`PublishFailedError`:
    Import them from :mod:`metricq.exceptions`, without the :literal:`...Error`-suffix:

    * :exc:`AgentStopped`
    * :exc:`ConnectFailed`
    * :exc:`ReceivedSignal`
    * :exc:`PublishFailed`

:code:`metricq.agent.RPCError`, :code:`metricq.history_client.InvalidHistoryResponse`:
    Moved to :mod:`metricq.exceptions`:

    * :exc:`RPCError`
    * :exc:`InvalidHistoryResponse`

Removed exceptions
^^^^^^^^^^^^^^^^^^

:code:`metricq.source.MetricSendError`, :code:`metricq.agent.RPCReplyError`, :code:`metricq.client.ManagementRpcPublishError`:
    Handle :exc:`PublishFailed` instead.

:code:`metricq.sink.SinkError`, :code:`metricq.sink.SinkResubscribeError`:
    Removed, the exception that caused them is raised directly.


.. py:currentmodule:: metricq

Validation of :class:`Source` chunk sizes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Previously, anything could be assigned to :attr:`Source.chunk_size`,
now only :literal:`None` (to disable automatic chunking)
and *positive* integers are accepted.


Removal of previously deprecated methods
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Support for tuple-unpacking instances of :class:`TimeAggregate` has been removed.
* The methods :code:`history_metric_list` and :code:`history_metric_metadata`
  on :class:`HistoryClient` have been removed.
  They are superseded by the more general :meth:`HistoryClient.get_metrics`;
  to upgrade, make the following changes:

    .. code-block:: diff

        client = HistoryClient(...)

        -await client.history_metric_list(...)
        +await client.get_metrics(...)

        -await client.history_metric_metadata(...)
        +await client.get_metrics(..., metadata=True)


Type changes to reduce ambiguity
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Some class attributes were raw numbers;
we gave them appropriate wrapper types that make it harder to accidentally misuse them.

:attr:`TimeAggregate.active_time` and :attr:`IntervalSource.period`:
    These are now proper durations (:class:`Timedelta`) instead of a raw numbers of seconds.

    >>> now = Timestamp.now()
    >>> agg = TimeAggregate.from_value_pair(now, now + Timedelta.from_ms(500), value=42.0)
    >>> agg.active_time.precise_string
    '500ms'


Deprecation of :code:`TimeAggregate.integral`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The property :code:`TimeAggregate.integral` returned the integral of values over the time
spanned by a :class:`TimeAggregate`.
To reduce confusion over which time unit is used to integrate,
we deprecated it and added :attr:`TimeAggregate.integral_s` and :attr:`TimeAggregate.integral_ns`
for seconds- and nanoseconds-based integrals, respectively.

`2.x` → `3.0`
-------------

Removal of :code:`TimeAggregate.integral`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
The property :code:`TimeAggregate.integral` returned the integral of values over the time
spanned by a :class:`TimeAggregate`.
This property was broken and thus removed, instead, use :attr:`TimeAggregate.integral_s` and :attr:`TimeAggregate.integral_ns`
for seconds- and nanoseconds-based integrals, respectively.

`3.x` → `4.0`
-------------

Dropping support for older Python versions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

With version 4.0, MetricQ-Python only supports Python **3.10 (and newer)**. You need to upgrade to
this Python version.

Removal of Asyncio Loop
^^^^^^^^^^^^^^^^^^^^^^^

Python 3.10 removed the loop parameter from the asyncio interface. In particual, the
constructor parameter `event_loop` of :class:`Agent` got removed, as well as,
the property :attr:`Agent.event_loop`.
