# ---------------------------
#            imgui
# ---------------------------

target_sources (mazeman PRIVATE
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_opengl3.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
)

target_include_directories(mazeman PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
