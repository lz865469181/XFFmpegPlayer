
#include <QtWidgets/QApplication>
#include "XDemux.h"
#include <iostream>
#include "XPlayer.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	XPlayer w;
	w.show();
	return a.exec();
}
