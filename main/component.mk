#
# Main component makefile.
#
# This Makefile can be left empty. By default, it will take the sources in the
# src/ directory, compile them and link them into lib(subdirectory_name).a
# in the build directory. This behaviour is entirely configurable,
# please read the ESP-IDF documents if you need to do this.
#

# Get a object such as TXT file that is embeded into Flash memory previously

COMPONENT_EMBED_TXTFILES := ../image/anh.jpg
COMPONENT_EMBED_TXTFILES := ../html/index.html
