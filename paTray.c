// I am not a programmer. This contains unused stuff from an example application.
// I just wanted a tray icon to launch pavucontrol on my i3 desktop.
// Use the below command to compile this app:
// gcc -o paTray $(pkg-config --cflags --libs gtk+-2.0) paTray.c
// To have the icon show up, you need to copy it from ./resources/paTray.png to /usr/share/icons/paTray.png

#include <gtk/gtk.h>
#include <stdio.h>

static void trayView(GtkMenuItem *item, gpointer user_data);
static void trayExit(GtkMenuItem *item, gpointer user_data);
static void trayIconActivated(GObject *trayIcon, gpointer data);
static void trayIconPopup(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu);
static void destroy (GtkWidget*, gpointer);
static gboolean delete_event (GtkWidget*, GdkEvent*, gpointer);
static gboolean window_state_event (GtkWidget *widget, GdkEventWindowState *event, gpointer user_data);

int main(int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Pulse Audio Control Tray");
    gtk_widget_set_size_request (window, 200, -1);

    //set try icon file
    GtkStatusIcon *trayIcon  = gtk_status_icon_new_from_file ("/usr/share/icons/paTray.png");
    //set popup menu for tray icon
    GtkWidget *menu, *menuItemView, *menuItemExit;
    menu = gtk_menu_new();
    menuItemView = gtk_menu_item_new_with_label ("Launch pavucontrol");
    menuItemExit = gtk_menu_item_new_with_label ("Exit");
    g_signal_connect (G_OBJECT (menuItemView), "activate", G_CALLBACK (trayView), window);
    g_signal_connect (G_OBJECT (menuItemExit), "activate", G_CALLBACK (trayExit), NULL);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuItemView);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuItemExit);
    gtk_widget_show_all (menu);
    //set tooltip
    gtk_status_icon_set_tooltip (trayIcon, "Pulse Audio Control Launcher");
    //connect handlers for mouse events
    //g_signal_connect(GTK_STATUS_ICON (trayIcon), "activate", GTK_SIGNAL_FUNC (trayIconActivated), window);
    g_signal_connect(GTK_STATUS_ICON (trayIcon), "popup-menu", GTK_SIGNAL_FUNC (trayIconPopup), menu);
    gtk_status_icon_set_visible(trayIcon, TRUE); //set icon initially visible


    GtkWidget *menuBar, *menuItemTopLvl, *mainMenu, *mainMenuItemExit;
    menuBar = gtk_menu_bar_new ();
    menuItemTopLvl = gtk_menu_item_new_with_label ("Menu");
    gtk_menu_shell_append (GTK_MENU_SHELL (menuBar), menuItemTopLvl);

    mainMenu = gtk_menu_new ();
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuItemTopLvl), mainMenu);
    mainMenuItemExit = gtk_menu_item_new_with_label ("Quit");
    g_signal_connect (G_OBJECT (mainMenuItemExit), "activate", G_CALLBACK (trayExit), NULL);
    gtk_menu_shell_append (GTK_MENU_SHELL (mainMenu), mainMenuItemExit);

    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy), NULL);
    g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (delete_event), trayIcon);
    g_signal_connect (G_OBJECT (window), "window-state-event", G_CALLBACK (window_state_event), trayIcon);
    gtk_container_add (GTK_CONTAINER (window), menuBar);
    //gtk_widget_show_all (window);
    gtk_main ();
    return 0;
}

static void trayView(GtkMenuItem *item, gpointer window)
{
    // Launch pavucontrol
    system("pavucontrol");
}

static void trayExit(GtkMenuItem *item, gpointer user_data)
{
    printf("exit");
    gtk_main_quit();
}

static void trayIconActivated(GObject *trayIcon, gpointer window)
{
      // uncomment to launch pavucontrol on click of tray icon
      // system("pavucontrol");
}

static void trayIconPopup(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu)
{
    gtk_menu_popup(GTK_MENU(popUpMenu), NULL, NULL, gtk_status_icon_position_menu, status_icon, button, activate_time);
}

static void destroy (GtkWidget *window, gpointer data)
{
  gtk_main_quit ();
}

static gboolean delete_event (GtkWidget *window, GdkEvent *event, gpointer data)
{
    return FALSE;
}

static gboolean window_state_event (GtkWidget *widget, GdkEventWindowState *event, gpointer trayIcon)
{
    if(event->changed_mask == GDK_WINDOW_STATE_ICONIFIED && (event->new_window_state == GDK_WINDOW_STATE_ICONIFIED || event->new_window_state == (GDK_WINDOW_STATE_ICONIFIED | GDK_WINDOW_STATE_MAXIMIZED)))
    {
        gtk_widget_hide (GTK_WIDGET(widget));
        gtk_status_icon_set_visible(GTK_STATUS_ICON(trayIcon), TRUE);
    }
    else if(event->changed_mask == GDK_WINDOW_STATE_WITHDRAWN && (event->new_window_state == GDK_WINDOW_STATE_ICONIFIED || event->new_window_state == (GDK_WINDOW_STATE_ICONIFIED | GDK_WINDOW_STATE_MAXIMIZED)))
    {
        gtk_status_icon_set_visible(GTK_STATUS_ICON(trayIcon), FALSE);
    }
    return TRUE;
}
