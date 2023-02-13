/*
______________________________________
* File:
  gui.c
* Date:
  10/08/2021
* Author:
  rafameou
* Notes:
* Purpose:
______________________________________
*/

#include "sdk.h"
#include "gui.h"

int sleepTime = 5;

typedef struct browserGUI
{
  GtkApplication *application;

  // Used in Main Window
  GtkWidget *window, *box,
      *menuBar,
      *fileMenu, *fMFile, *fMQuit,
      *helpMenu, *fMHelp, *fMAbout,
      *hBox,
      *status, *name, *date, *die,
      *stop, *copyright;
  GdkPixbuf *logoIcon;
} tBrowserGUI;

tBrowserGUI brGui;

void closeEverything()
{
  // TODO: limpar as coisas
  gtk_window_close(GTK_WINDOW(brGui.window));
  gtk_widget_destroy(brGui.window);
  gtk_main_quit();
}

void showAbout()
{
  gtk_show_about_dialog(GTK_WINDOW(brGui.window),
                        "program-name", "bing-mate",
                        //"version", "0.0",
                        "title", "About bing-mate",
                        "copyright", "Copyright \xc2\xa9 2023 rafameou",
                        "comments", "Bing wallpaper changer for mate.",
                        "logo", brGui.logoIcon,
                        NULL);
}

void setupToolBar()
{

  // --- MENUBAR
  brGui.menuBar = gtk_menu_bar_new();

  brGui.fileMenu = gtk_menu_new();
  brGui.fMFile = gtk_menu_item_new_with_label("File");
  brGui.fMQuit = gtk_menu_item_new_with_label("Quit");

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(brGui.fMFile), brGui.fileMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(brGui.fileMenu), brGui.fMQuit);
  g_signal_connect(G_OBJECT(brGui.fMQuit), "activate", G_CALLBACK(closeEverything), brGui.window);
  gtk_menu_shell_append(GTK_MENU_SHELL(brGui.menuBar), brGui.fMFile);

  brGui.helpMenu = gtk_menu_new();
  brGui.fMHelp = gtk_menu_item_new_with_label("Help");
  brGui.fMAbout = gtk_menu_item_new_with_label("About");

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(brGui.fMHelp), brGui.helpMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(brGui.helpMenu), brGui.fMAbout);
  g_signal_connect(G_OBJECT(brGui.fMAbout), "activate", G_CALLBACK(showAbout), brGui.window);
  gtk_menu_shell_append(GTK_MENU_SHELL(brGui.menuBar), brGui.fMHelp);

  gtk_box_pack_start(GTK_BOX(brGui.box), brGui.menuBar, FALSE, FALSE, 0);
}

char copyurl[1024] = "xdg-open ";
void openBrowser()
{
  // Find a better way to do this.
  system(copyurl);
}

void stop()
{
  sleepTime = -1;
}

void setupBottomBar()
{
  // brGui.progressBar = gtk_progress_bar_new();
  //  gtk_widget_set_name(GTK_WIDGET(brGui.progressBar), "progressbar");
  // gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(brGui.progressBar), FALSE);
  // gtk_box_pack_start(GTK_BOX(brGui.box), GTK_WIDGET(brGui.progressBar), FALSE, FALSE, 0);

  brGui.hBox = gtk_box_new(0, 0);
  gtk_box_pack_start(GTK_BOX(brGui.box), brGui.hBox, TRUE, TRUE, 5);

  brGui.stop = gtk_button_new_with_label("Stop Timer");
  gtk_box_pack_start(GTK_BOX(brGui.hBox), GTK_WIDGET(brGui.stop), FALSE, FALSE, 0);
  g_signal_connect(G_OBJECT(brGui.stop), "clicked", G_CALLBACK(stop), brGui.window);

  brGui.copyright = gtk_button_new_with_label("Open on Bing");
  gtk_box_pack_end(GTK_BOX(brGui.hBox), GTK_WIDGET(brGui.copyright), FALSE, FALSE, 0);
  g_signal_connect(G_OBJECT(brGui.copyright), "clicked", G_CALLBACK(openBrowser), brGui.window);
}

void setupWindow()
{
  gtk_window_set_position(GTK_WINDOW(brGui.window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(brGui.window), 0, 0);

  brGui.box = gtk_box_new(1, 0);
  gtk_container_add(GTK_CONTAINER(brGui.window), brGui.box);
}

struct memoryStruct
{
  char *content;
  size_t size;
};

static size_t writeMemory(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct memoryStruct *memory = (struct memoryStruct *)userp;

  char *ptr = realloc(memory->content, memory->size + realsize + 1);
  if (ptr)
  {
    memory->content = ptr;
    memcpy(&(memory->content[memory->size]), contents, realsize);
    memory->size += realsize;
    memory->content[memory->size] = 0;
    return realsize;
  }

  return 0;
}

void *runProgram()
{
  FILE *fp;
  char pictures[1024];
  fp = popen("xdg-user-dir PICTURES", "r");
  if (fp == NULL || fgets(pictures, 1024, fp) == NULL)
  {
    gtk_label_set_text(GTK_LABEL(brGui.status), "Failed to open pictures folder.");
    return NULL;
  }
  pictures[strcspn(pictures, "\n")] = '/';
  pclose(fp);

  CURLcode res;
  struct stat status = {0};

  curl_global_init(CURL_GLOBAL_DEFAULT);

  CURL *curl = curl_easy_init();
  json_object *JSON;
  enum json_tokener_error JSON_ERROR = json_tokener_success;

  struct memoryStruct chunk;

  chunk.content = malloc(1);
  chunk.size = 0;

  // gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(brGui.progressBar), 0.1);

  if (curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL, "https://www.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1&mkt=en-US");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemory);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    res = curl_easy_perform(curl);
    // gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(brGui.progressBar), 0.25);
    gtk_label_set_text(GTK_LABEL(brGui.status), "Downloading image...");
    if (res == CURLE_OK)
    {
      JSON = json_tokener_parse_verbose(chunk.content, &JSON_ERROR);
      free(chunk.content);
      char url[1024] = "https://www.bing.com";
      json_object *access = json_object_object_get(JSON, "images");
      access = json_object_array_get_idx(access, 0);
      strcat(url, json_object_get_string(json_object_object_get(access, "url")));
      strcat(copyurl, json_object_get_string(json_object_object_get(access, "copyrightlink")));

      gtk_label_set_text(GTK_LABEL(brGui.name), json_object_get_string(json_object_object_get(access, "title")));
      gtk_label_set_text(GTK_LABEL(brGui.date), json_object_get_string(json_object_object_get(access, "startdate")));

      strcat(pictures, "bing-mate/");
      if (stat(pictures, &status) == -1)
        mkdir(pictures, 0777);
      strcat(pictures, json_object_get_string(json_object_object_get(access, "startdate")));
      strcat(pictures, ".jpg");

      if (stat(pictures, &status) == -1)
      {
        FILE *file = fopen(pictures, "w");

        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        res = curl_easy_perform(curl);
        if (res == CURLE_OK)
          gtk_label_set_text(GTK_LABEL(brGui.status), "Done.");
      }
      else
      {
        gtk_label_set_text(GTK_LABEL(brGui.status), "Already downloaded.");
      }

      GSettings *wallpaperSets = g_settings_new("org.mate.background");
      g_settings_set_string(wallpaperSets, "picture-filename", pictures);

      // gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(brGui.progressBar), 1.0);

      json_object_put(JSON);
    }
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  if (res != CURLE_OK)
  {
    gtk_label_set_text(GTK_LABEL(brGui.status), "Failed to curl.");
    // gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(brGui.progressBar), 1.0);
  }

  for (int i = sleepTime; i >= 0; i--)
  {
    if (sleepTime == -1)
      return NULL;
    gtk_label_set_text(GTK_LABEL(brGui.die), i == 0 ? "Bye!" : g_strdup_printf("Exiting in %d seconds.", i));
    sleep(1);
  }

  closeEverything();
  return NULL;
}

void startBrowser(GtkApplication *application)
{
  brGui.application = application;
  brGui.window = gtk_application_window_new(brGui.application);

  brGui.logoIcon = gtk_icon_theme_load_icon(gtk_icon_theme_get_default(),
                                            "preferences-desktop-wallpaper", 64,
                                            GTK_ICON_LOOKUP_NO_SVG, NULL);

  gtk_window_set_icon(GTK_WINDOW(brGui.window), brGui.logoIcon);
  gtk_window_set_title(GTK_WINDOW(brGui.window), g_strdup_printf("bing-mate"));

  setupWindow();
  setupToolBar();

  //brGui.hBox = gtk_box_new(1, 0);
  //gtk_box_pack_start(GTK_BOX(brGui.box), brGui.hBox, TRUE, TRUE, 0);

  brGui.status = gtk_label_new("Loading... Please wait.");
  gtk_container_add(GTK_CONTAINER(brGui.box), brGui.status);

  brGui.name = gtk_label_new("Title");
  gtk_container_add(GTK_CONTAINER(brGui.box), brGui.name);

  brGui.date = gtk_label_new("Date");
  gtk_container_add(GTK_CONTAINER(brGui.box), brGui.date);

  brGui.die = gtk_label_new("...");
  gtk_container_add(GTK_CONTAINER(brGui.box), brGui.die);

  pthread_t thread;
  pthread_create(&thread, NULL, runProgram, NULL);

  setupBottomBar();

  g_signal_connect(G_OBJECT(brGui.window), "destroy", closeEverything, NULL);
  gtk_widget_show_all(brGui.window);
}