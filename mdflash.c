#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

// Load CSS styles for the text view
static void load_css(GtkTextView *text_view) {
    // Create a new CSS provider
    GtkCssProvider *provider = gtk_css_provider_new();
    
    // Get the default display and screen
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);
    
    // Add the CSS provider to the screen
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    // Define the CSS styles
    const char *css = "textview { background-color: white; font-size: 16px; }"
                      "textview text { background-color: white; }"
                      "textview text:selected { background-color: #3399ff; }"
                      ".code-block { background-color: #f6f8fa; font-family: monospace; }"
                      ".heading { background-color: #f0f0f0; }"
                      ".thin-line { background-color: #f0f0f0; min-height: 1px; }"
                      ".container-box { background-color: white; }";
    
    // Load the CSS data into the provider
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    
    // Unreference the provider
    g_object_unref(provider);
}

// Display the contents of the README.md file in a text view
static void display_readme(const char *filename) {
    // Create a new top-level window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    // Set the window title
    gtk_window_set_title(GTK_WINDOW(window), "README.md Viewer");
    
    // Set the default window size
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Create a new text view
    GtkWidget *text_view = gtk_text_view_new();
    
    // Set the text view to be non-editable
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    
    // Hide the cursor in the text view
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    
    // Set the text wrapping mode to word wrapping
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);

    // Get the text buffer associated with the text view
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    
    // Get the start iterator of the text buffer
    GtkTextIter iter;
    gtk_text_buffer_get_start_iter(buffer, &iter);

    // Open the README.md file for reading
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(1);
    }

    // Initialize variables for reading the file
    char line[MAX_LINE_LENGTH];
    gboolean in_code_block = FALSE;

    // Read the file line by line and apply appropriate formatting
    while (fgets(line, sizeof(line), file) != NULL) {
        // Check if the line starts a code block
        if (strncmp(line, "```", 3) == 0) {
            if (!in_code_block) {
                // Insert a newline character
                gtk_text_buffer_insert(buffer, &iter, "\n", -1);
                
                // Set the code block flag to true
                in_code_block = TRUE;
            } else {
                // Insert a newline character
                gtk_text_buffer_insert(buffer, &iter, "\n", -1);
                
                // Set the code block flag to false
                in_code_block = FALSE;
            }
        }
        // Check if the line is inside a code block
        else if (in_code_block) {
            // Create a tag for code block formatting
            GtkTextTag *tag = gtk_text_buffer_create_tag(buffer, NULL,
                                                         "background", "#f6f8fa",
                                                         "family", "monospace",
                                                         NULL);
            
            // Insert the line with the code block tag
            gtk_text_buffer_insert_with_tags(buffer, &iter, line, -1, tag, NULL);
        }
        // Check if the line starts with "# " (h1 heading)
        else if (strncmp(line, "# ", 2) == 0) {
            // Create a tag for h1 heading formatting
            GtkTextTag *tag = gtk_text_buffer_create_tag(buffer, NULL,
                                                         "weight", PANGO_WEIGHT_BOLD,
                                                         "size", 22 * PANGO_SCALE,
                                                         NULL);
            
            // Insert the line with the h1 heading tag
            gtk_text_buffer_insert_with_tags(buffer, &iter, line + 2, -1, tag, NULL);
            
            // Insert a newline character
            gtk_text_buffer_insert(buffer, &iter, "", -1);
        }
        // Check if the line starts with "## " (h2 heading)
        else if (strncmp(line, "## ", 3) == 0) {
            // Create a tag for h2 heading formatting
            GtkTextTag *tag = gtk_text_buffer_create_tag(buffer, NULL,
                                                         "weight", PANGO_WEIGHT_BOLD,
                                                         "size", 16 * PANGO_SCALE,
                                                         NULL);
            
            // Insert the line with the h2 heading tag
            gtk_text_buffer_insert_with_tags(buffer, &iter, line + 3, -1, tag, NULL);
            
            // Insert a newline character
            gtk_text_buffer_insert(buffer, &iter, "", -1);
            
            // Create a tag for a thin line
            GtkTextTag *line_tag = gtk_text_buffer_create_tag(buffer, NULL,
                                                              "background", "#f0f0f0",
                                                              "paragraph-background", "#f0f0f0",
                                                              "size", 1 * PANGO_SCALE,
                                                              NULL);
            
            // Insert a newline character with the thin line tag
            gtk_text_buffer_insert_with_tags(buffer, &iter, "\n", -1, line_tag, NULL);
        }
        // Check if the line starts with "### " (h3 heading)
        else if (strncmp(line, "### ", 4) == 0) {
            // Create a tag for h3 heading formatting
            GtkTextTag *tag = gtk_text_buffer_create_tag(buffer, NULL,
                                                         "weight", PANGO_WEIGHT_BOLD,
                                                         "size", 14 * PANGO_SCALE,
                                                         NULL);
            
            // Insert the line with the h3 heading tag
            gtk_text_buffer_insert_with_tags(buffer, &iter, line + 4, -1, tag, NULL);
        }
        // For all other lines
        else {
            // Insert the line without any formatting
            gtk_text_buffer_insert(buffer, &iter, line, -1);
        }
    }

    // Close the file
    fclose(file);

    // Create a new scrolled window
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    
    // Add the text view to the scrolled window
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

    // Create a new box container to hold the scrolled window
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // Add padding to the box container
    gtk_container_set_border_width(GTK_CONTAINER(box), 40);

    // Set the border width of the scrolled window
    gtk_container_set_border_width(GTK_CONTAINER(scrolled_window), 10);

    // Add a CSS class to the box container
    GtkStyleContext *context = gtk_widget_get_style_context(box);
    gtk_style_context_add_class(context, "container-box");

    // Add the scrolled window to the box container
    gtk_box_pack_start(GTK_BOX(box), scrolled_window, TRUE, TRUE, 0);
    
    // Add the box container to the main window
    gtk_container_add(GTK_CONTAINER(window), box);

    // Load the CSS styles for the text view
    load_css(GTK_TEXT_VIEW(text_view));

    // Connect the "destroy" signal to the main loop quit function
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Show all widgets in the window
    gtk_widget_show_all(window);
    
    // Run the main loop
    gtk_main();
}

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <README.md>\n", argv[0]);
        return 1;
    }

    // Initialize GTK
    gtk_init(&argc, &argv);
    
    // Display the README.md file
    display_readme(argv[1]);

    return 0;
}
