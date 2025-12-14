# SuperBLT DLL Changelog

This lists the changes between different versions of the SuperBLT DLL,
the changes for the basemod are listed in their own changelog.
Contributors other than maintainers are listed in parenthesis after specific changes.

## Version 1.1.0.1
hotfix for zlib-ng v2.3.x breaking builds

## Version 1.1.0.0

updated curl, wren and zlib-ng
added external updater for dll

## Version 1.0.1.0

blt.asset_db rewrite for RAID
updated curl, wren and replaced zlib with zlib-ng

## Version 1.0.0.3

new signatures for U25

## Version 1.0.0.2

new signatures for U24.4

## Version 1.0.0.1

added check to prevent crashing the game after updates

## Version 1.0.0.0

based on PD2-SuperBLT DLL v3.3.8
patched for raid, removed linux/w32/pd2/vr codes
included luajit from leon's repo
Removed XAudio API
updated libcurl with enabled http(s) features: alt-svc AsynchDNS HSTS HTTPS-proxy IPv6 Largefile libz SSL SSPI threadsafe
updated mxml
updated subhook
updated wren
updated libpng
updated zlib, and linked curl against zlib
removed openssl - replaced sha256 impl. with Windows CNG API impl.