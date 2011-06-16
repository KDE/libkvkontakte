#! /usr/bin/env bash
$EXTRACTRC *.kcfg >> rc.cpp
$XGETTEXT *.cpp -o $podir/akonadi_vkontakte_resource.pot
rm -f rc.cpp
