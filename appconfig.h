#ifndef APPCONFIG_H
#define APPCONFIG_H

struct app_config{
    bool notetree_alternating_row_colors;
    bool notetree_select_rows;
    int notetree_indentation_size;
    int opacity; //100 for solid, 0 for transparent
    bool notetree_animated;
    bool doubleClickToEditMessage;
    unsigned short int layoutMargin;
    unsigned short int splitter_handle_width;
    bool confirm_delete;
    bool line_wrapping;
    bool pair_completion;
    bool notetree_drag_drop;
    int maximum_backups;
};

#endif // APPCONFIG_H
