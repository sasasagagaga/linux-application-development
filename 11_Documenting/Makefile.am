bin_PROGRAMS=guesser
pkgdata_DATA=ru/LC_MESSAGES/guesser.mo guesser.1

@DX_RULES@

all-local: doxygen-doc

%.1: %
	-$(HELP2MAN) --no-info --output=$@ --name='@PACKAGE_NAME@' ./$<

ru/LC_MESSAGES/guesser.mo: ru.po
	mkdir -p `dirname $@`
	msgfmt $< -o $@

ru.po: messages.pot
	msgmerge -U $@ $<
	rm $<

messages.pot: guesser.pot guesser.c
	xgettext -k_ -c guesser.c -o messages.new.pot
	cp guesser.pot messages.pot
	msgmerge -U messages.pot messages.new.pot
	rm messages.new.pot

gitclean:
	git clean -df

