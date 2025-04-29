gtk: main.c trigram_table.c lettercheck.c
	clear && gcc `pkg-config --cflags --libs gtk+-2.0` -o acgc main.c trigram_table.c lettercheck.c
