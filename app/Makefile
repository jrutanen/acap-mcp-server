PROG1	= mcpserver
SRC_DIRS = src src/tools
OBJS1 = $(shell find $(SRC_DIRS) -name '*.c')
PROGS	= $(PROG1)
MONKEY_DIR = /opt/build/monkey/build
MONKEY_CONF_DIR = /workspaces/acap-mcp-server/monkey_conf
START_SCRIPT_DIR = /workspaces/acap-mcp-server/scripts
LIBDIR = lib
HTMLDIR = html

PKGS = glib-2.0 monkey jansson
CFLAGS += $(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags $(PKGS))
LDLIBS += $(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs $(PKGS))

CFLAGS += -I$(MONKEY_DIR)/include -Iinclude
LDFLAGS += -L./$(LIBDIR)
LDFLAGS += -Wl,-rpath,'$$ORIGIN/$(LIBDIR)'
LDFLAGS += -Wl,-rpath,'$$ORIGIN/lib/plugins'

CFLAGS += -Wall \
          -Wextra \
          -Wformat=2 \
          -Wpointer-arith \
          -Wbad-function-cast \
          -Wstrict-prototypes \
          -Wmissing-prototypes \
          -Winline \
          -Wdisabled-optimization \
          -Wfloat-equal \
          -W \
          -Werror

all:	$(PROGS)
$(PROG1): $(OBJS1)
	cp -r $(MONKEY_DIR)/$(LIBDIR) .
	cp -a /usr/local/packages/mcpserver/lib/plugins/*.so ./lib/plugins/
	mkdir -p $(HTMLDIR)
	cp -r $(MONKEY_CONF_DIR)/* $(HTMLDIR)/
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -rf $(PROGS) *.o *.eap* *_LICENSE.txt package.conf* param.conf tmp* $(LIBDIR) $(HTMLDIR) build/*