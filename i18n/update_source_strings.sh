#!/bin/bash

# This script updates the openrpgeditor.pot file to match the strings in
# the source code.

cd "$(dirname "$0")/.."

# Scan the source code for strings and put them in openrpgeditor.pot.pot
SRCDIR=src
find $SRCDIR -name '*.cpp' -o -name '*.h' -o -name '*.hpp' -o -name '*.inl' -o -name '*.c' | \
	xgettext -p ./i18n/locales -o openrpgeditor.pot --package-name="OpenRPG Editor" \
	--keyword=tr \
	--keyword=trFormat \
	--keyword=trNOOP \
	--keyword=trENUM \
	--add-comments=i18n \
	--add-comments=TL-NOTE \
	--from-code=utf-8\
	--copyright-holder="OpenRPGEditor Team" \
	--language=c++ -f -

sed -i "s/SOME DESCRIPTIVE TITLE\./Translation of openrpgeditor.pot to LANGUAGE/" i18n/locales/openrpgeditor.pot
sed -i "s/YEAR THE PACKAGE'S COPYRIGHT HOLDER/2024-2025/" i18n/locales/openrpgeditor.pot
sed -i "s/license as the PACKAGE package/license as the openrpgeditor package/" i18n/locales/openrpgeditor.pot
