add_library(sb_app app.c)
target_link_libraries(sb_app sb_blob)
target_link_libraries(sb_app input_controller)
