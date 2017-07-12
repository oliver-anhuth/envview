DESTDIR       =
BINDIR        = /usr/bin
ICONDIR       = /usr/share/icons
APPDIR        = /usr/share/applications

TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

install:
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(MKDIR) $(DESTDIR)$(ICONDIR)
	$(MKDIR) $(DESTDIR)$(APPDIR)
	$(INSTALL_PROGRAM) envview $(DESTDIR)$(BINDIR)/envview
	$(INSTALL_FILE) envview.png $(DESTDIR)$(ICONDIR)/envview.png
	$(INSTALL_FILE) envview.desktop $(DESTDIR)$(APPDIR)/envview.desktop

uninstall:
	$(DEL_FILE) $(DESTDIR)$(BINDIR)/envview
	$(DEL_FILE) $(DESTDIR)$(ICONDIR)/envview.png
	$(DEL_FILE) $(DESTDIR)$(APPDIR)/envview.desktop
