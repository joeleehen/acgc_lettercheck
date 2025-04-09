gtk: main.c trigram_table.c lettercheck.c
	gcc `pkg-config --cflags --libs gtk+-2.0` main.c trigram_table.c lettercheck.c
