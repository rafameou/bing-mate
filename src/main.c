/*
______________________________________
* File:
  main.c
* Date:
  09/08/2021
* Author:
  rafameou
* Notes:
* Purpose:
______________________________________
*/

#include "sdk.h"
#include "gui.h"

int main(int argc, char **argv)
{
  GtkApplication *mainWindow;
  int status;

  mainWindow = gtk_application_new("rafameou.bing-mate", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(mainWindow, "activate", G_CALLBACK(startBrowser), NULL);
  status = g_application_run(G_APPLICATION(mainWindow), argc, argv);
  g_object_unref(mainWindow);
  return status;
}
