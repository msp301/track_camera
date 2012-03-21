#include "GuiMainWindow.hpp"
#include "ui_GuiMainWindow.h"

GuiMainWindow::GuiMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::GuiMainWindow)
{
	ui->setupUi(this);
}

GuiMainWindow::~GuiMainWindow()
{
	delete ui;
}
