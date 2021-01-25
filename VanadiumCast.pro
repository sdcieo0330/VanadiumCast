TEMPLATE = subdirs

SUBDIRS += backend \
           VanadiumCast \
	   QtAV

backend.depends = QtAV
VanadiumCast.depends = backend
