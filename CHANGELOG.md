# Changelog

## [v1.2.0](https://github.com/metricq/metricq-python/tree/v1.2.0) (2020-08-03)

[Full Changelog](https://github.com/metricq/metricq-python/compare/v1.1.4...v1.2.0)

**Implemented enhancements:**

- Add metricq-spy tool, which allows to search for historic metrics
- Add "hostname" to discover response [\#19](https://github.com/metricq/metricq-python/issues/19)
- discover RPC: rename "version" field to "metricqVersion" [\#15](https://github.com/metricq/metricq-python/issues/15)
- Add field "hostname" to discover response [\#20](https://github.com/metricq/metricq-python/pull/20) ([phijor](https://github.com/phijor))

**Fixed bugs:**

- Add setuptools as runtime dependecies [\#14](https://github.com/metricq/metricq-python/issues/14)
- Protobuf version conflict [\#10](https://github.com/metricq/metricq-python/issues/10)
- Python agents fail to anwser discover rpc [\#8](https://github.com/metricq/metricq-python/issues/8)

**Merged pull requests:**

- Use protobuf version from compile time [\#21](https://github.com/metricq/metricq-python/pull/21) ([kinnarr](https://github.com/kinnarr))
- Fix package version related issues [\#16](https://github.com/metricq/metricq-python/pull/16) ([phijor](https://github.com/phijor))

## [v1.1.4](https://github.com/metricq/metricq-python/tree/v1.1.4) (2020-06-24)

[Full Changelog](https://github.com/metricq/metricq-python/compare/v1.1.3...v1.1.4)

**Closed issues:**

- Fix protobuf building [\#2](https://github.com/metricq/metricq-python/issues/2)
- Create repo metricq-python [\#1](https://github.com/metricq/metricq-python/issues/1)

**Merged pull requests:**

- Fix protobuf compilation [\#5](https://github.com/metricq/metricq-python/pull/5) ([phijor](https://github.com/phijor))

## [v1.1.3](https://github.com/metricq/metricq-python/tree/v1.1.3) (2020-03-23)

[Full Changelog](https://github.com/metricq/metricq-python/compare/v1.1.2...v1.1.3)

**Merged pull requests:**

- Cleanup shutdown [\#53](https://github.com/metricq/metricq/pull/53) ([tilsche](https://github.com/tilsche))
- GitHub actions cpp [\#52](https://github.com/metricq/metricq/pull/52) ([bmario](https://github.com/bmario))

## [v1.1.2](https://github.com/metricq/metricq-python/tree/v1.1.2) (2020-02-21)

[Full Changelog](https://github.com/metricq/metricq-python/compare/v1.1.1...v1.1.2)

**Merged pull requests:**

- Adds github action for python tests and release [\#51](https://github.com/metricq/metricq/pull/51) ([kinnarr](https://github.com/kinnarr))

## [v1.1.1](https://github.com/metricq/metricq-python/tree/v1.1.1) (2020-02-21)

[Full Changelog](https://github.com/metricq/metricq-python/compare/v1.1.0...v1.1.1)

**Fixed bugs:**

- metricq won't build with -Werror due to deprecated history.proto items [\#49](https://github.com/metricq/metricq/issues/49)
- connection reset by peer - channel robustness in python [\#38](https://github.com/metricq/metricq/issues/38)
- Invalid state after robust reconnect [\#23](https://github.com/metricq/metricq/issues/23)

## [v1.1.0](https://github.com/metricq/metricq-python/tree/v1.1.0) (2019-12-10)

[Full Changelog](https://github.com/metricq/metricq-python/compare/v1.0.0...v1.1.0)

**Merged pull requests:**

- Robust Agents: make Agents survive reconnects [\#46](https://github.com/metricq/metricq/pull/46) ([phijor](https://github.com/phijor))
- Improvements for docker deployment [\#45](https://github.com/metricq/metricq/pull/45) ([kinnarr](https://github.com/kinnarr))
- More config options for dummy sink and source [\#43](https://github.com/metricq/metricq/pull/43) ([kinnarr](https://github.com/kinnarr))

## [v1.0.0](https://github.com/metricq/metricq-python/tree/v1.0.0) (2019-10-01)

[Full Changelog](https://github.com/metricq/metricq-python/compare/f3ad7c612e87569afc26f44268c2e4c4dc93161b...v1.0.0)

- Initial release

\* *This Changelog was automatically generated by [github_changelog_generator](https://github.com/github-changelog-generator/github-changelog-generator)*