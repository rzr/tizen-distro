# For compatibility
def base_path_join(a, *p):
    return oe.path.join(a, *p)

def base_path_relative(src, dest):
    return oe.path.relative(src, dest)

def base_path_out(path, d):
    return oe.path.format_display(path, d)

def base_read_file(filename):
    return oe.utils.read_file(filename)

def base_ifelse(condition, iftrue = True, iffalse = False):
    return oe.utils.ifelse(condition, iftrue, iffalse)

def base_conditional(variable, checkvalue, truevalue, falsevalue, d):
    return oe.utils.conditional(variable, checkvalue, truevalue, falsevalue, d)

def base_less_or_equal(variable, checkvalue, truevalue, falsevalue, d):
    return oe.utils.less_or_equal(variable, checkvalue, truevalue, falsevalue, d)

def base_version_less_or_equal(variable, checkvalue, truevalue, falsevalue, d):
    return oe.utils.version_less_or_equal(variable, checkvalue, truevalue, falsevalue, d)

def base_contains(variable, checkvalues, truevalue, falsevalue, d):
    return oe.utils.contains(variable, checkvalues, truevalue, falsevalue, d)

def base_both_contain(variable1, variable2, checkvalue, d):
    return oe.utils.both_contain(variable1, variable2, checkvalue, d)

def base_prune_suffix(var, suffixes, d):
    return oe.utils.prune_suffix(var, suffixes, d)

def oe_filter(f, str, d):
    return oe.utils.str_filter(f, str, d)

def oe_filter_out(f, str, d):
    return oe.utils.str_filter_out(f, str, d)

def machine_paths(d):
    """List any existing machine specific filespath directories"""
    machine = d.getVar("MACHINE", True)
    filespathpkg = d.getVar("FILESPATHPKG", True).split(":")
    for basepath in d.getVar("FILESPATHBASE", True).split(":"):
        for pkgpath in filespathpkg:
            machinepath = os.path.join(basepath, pkgpath, machine)
            if os.path.isdir(machinepath):
                yield machinepath

def is_machine_specific(d):
    """Determine whether the current recipe is machine specific"""
    machinepaths = set(machine_paths(d))
    urldatadict = bb.fetch.init(d.getVar("SRC_URI", True).split(), d, True)
    for urldata in (urldata for urldata in urldatadict.itervalues()
                    if urldata.type == "file"):
        if any(urldata.localpath.startswith(mp + "/") for mp in machinepaths):
            return True

def oe_popen_env(d):
    env = d.getVar("__oe_popen_env", False)
    if env is None:
        env = {}
        for v in d.keys():
            if d.getVarFlag(v, "export"):
                env[v] = d.getVar(v, True) or ""
        d.setVar("__oe_popen_env", env)
    return env

def oe_run(d, cmd, **kwargs):
    import oe.process
    kwargs["env"] = oe_popen_env(d)
    return oe.process.run(cmd, **kwargs)

def oe_popen(d, cmd, **kwargs):
    import oe.process
    kwargs["env"] = oe_popen_env(d)
    return oe.process.Popen(cmd, **kwargs)

def oe_system(d, cmd, **kwargs):
    """ Popen based version of os.system. """
    if not "shell" in kwargs:
        kwargs["shell"] = True
    return oe_popen(d, cmd, **kwargs).wait()

# for MD5/SHA handling
def base_chk_load_parser(config_paths):
    import ConfigParser
    parser = ConfigParser.ConfigParser()
    if len(parser.read(config_paths)) < 1:
        raise ValueError("no ini files could be found")

    return parser

def base_chk_file(parser, pn, pv, src_uri, localpath, data):
    no_checksum = False
    # Try PN-PV-SRC_URI first and then try PN-SRC_URI
    # we rely on the get method to create errors
    pn_pv_src = "%s-%s-%s" % (pn,pv,src_uri)
    pn_src    = "%s-%s" % (pn,src_uri)
    if parser.has_section(pn_pv_src):
        md5    = parser.get(pn_pv_src, "md5")
        sha256 = parser.get(pn_pv_src, "sha256")
    elif parser.has_section(pn_src):
        md5    = parser.get(pn_src, "md5")
        sha256 = parser.get(pn_src, "sha256")
    elif parser.has_section(src_uri):
        md5    = parser.get(src_uri, "md5")
        sha256 = parser.get(src_uri, "sha256")
    else:
        no_checksum = True

    # md5 and sha256 should be valid now
    if not os.path.exists(localpath):
        bb.note("The localpath does not exist '%s'" % localpath)
        raise Exception("The path does not exist '%s'" % localpath)


    # Calculate the MD5 and 256-bit SHA checksums
    md5data = bb.utils.md5_file(localpath)
    shadata = bb.utils.sha256_file(localpath)

    # sha256_file() can return None if we are running on Python 2.4 (hashlib is
    # 2.5 onwards, sha in 2.4 is 160-bit only), so check for this and call the
    # standalone shasum binary if required.
    if shadata is None:
        try:
            shapipe = os.popen('PATH=%s oe_sha256sum %s' % (bb.data.getVar('PATH', data, True), localpath))
            shadata = (shapipe.readline().split() or [ "" ])[0]
            shapipe.close()
        except OSError:
            raise Exception("Executing shasum failed, please build shasum-native")
    
    if no_checksum == True:	# we do not have conf/checksums.ini entry
        try:
            file = open("%s/checksums.ini" % bb.data.getVar("TMPDIR", data, 1), "a")
        except:
            return False

        if not file:
            raise Exception("Creating checksums.ini failed")
        
        file.write("[%s]\nmd5=%s\nsha256=%s\n\n" % (src_uri, md5data, shadata))
        file.close()
        return False

    if not md5 == md5data:
        bb.note("The MD5Sums did not match. Wanted: '%s' and Got: '%s'" % (md5,md5data))
        raise Exception("MD5 Sums do not match. Wanted: '%s' Got: '%s'" % (md5, md5data))

    if not sha256 == shadata:
        bb.note("The SHA256 Sums do not match. Wanted: '%s' Got: '%s'" % (sha256,shadata))
        raise Exception("SHA256 Sums do not match. Wanted: '%s' Got: '%s'" % (sha256, shadata))

    return True

oe_soinstall() {
	# Purpose: Install shared library file and
	#          create the necessary links
	# Example:
	#
	# oe_
	#
	#oenote installing shared library $1 to $2
	#
	libname=`basename $1`
	install -m 755 $1 $2/$libname
	sonamelink=`${HOST_PREFIX}readelf -d $1 |grep 'Library soname:' |sed -e 's/.*\[\(.*\)\].*/\1/'`
	solink=`echo $libname | sed -e 's/\.so\..*/.so/'`
	ln -sf $libname $2/$sonamelink
	ln -sf $libname $2/$solink
}

oe_libinstall() {
	# Purpose: Install a library, in all its forms
	# Example
	#
	# oe_libinstall libltdl ${STAGING_LIBDIR}/
	# oe_libinstall -C src/libblah libblah ${D}/${libdir}/
	dir=""
	libtool=""
	silent=""
	require_static=""
	require_shared=""
	staging_install=""
	while [ "$#" -gt 0 ]; do
		case "$1" in
		-C)
			shift
			dir="$1"
			;;
		-s)
			silent=1
			;;
		-a)
			require_static=1
			;;
		-so)
			require_shared=1
			;;
		-*)
			oefatal "oe_libinstall: unknown option: $1"
			;;
		*)
			break;
			;;
		esac
		shift
	done

	libname="$1"
	shift
	destpath="$1"
	if [ -z "$destpath" ]; then
		oefatal "oe_libinstall: no destination path specified"
	fi
	if echo "$destpath/" | egrep '^${STAGING_LIBDIR}/' >/dev/null
	then
		staging_install=1
	fi

	__runcmd () {
		if [ -z "$silent" ]; then
			echo >&2 "oe_libinstall: $*"
		fi
		$*
	}

	if [ -z "$dir" ]; then
		dir=`pwd`
	fi

	dotlai=$libname.lai

	# Sanity check that the libname.lai is unique
	number_of_files=`(cd $dir; find . -name "$dotlai") | wc -l`
	if [ $number_of_files -gt 1 ]; then
		oefatal "oe_libinstall: $dotlai is not unique in $dir"
	fi


	dir=$dir`(cd $dir;find . -name "$dotlai") | sed "s/^\.//;s/\/$dotlai\$//;q"`
	olddir=`pwd`
	__runcmd cd $dir

	lafile=$libname.la

	# If such file doesn't exist, try to cut version suffix
	if [ ! -f "$lafile" ]; then
		libname1=`echo "$libname" | sed 's/-[0-9.]*$//'`
		lafile1=$libname.la
		if [ -f "$lafile1" ]; then
			libname=$libname1
			lafile=$lafile1
		fi
	fi

	if [ -f "$lafile" ]; then
		# libtool archive
		eval `cat $lafile|grep "^library_names="`
		libtool=1
	else
		library_names="$libname.so* $libname.dll.a $libname.*.dylib"
	fi

	__runcmd install -d $destpath/
	dota=$libname.a
	if [ -f "$dota" -o -n "$require_static" ]; then
		rm -f $destpath/$dota
		__runcmd install -m 0644 $dota $destpath/
	fi
	if [ -f "$dotlai" -a -n "$libtool" ]; then
		if test -n "$staging_install"
		then
			# stop libtool using the final directory name for libraries
			# in staging:
			__runcmd rm -f $destpath/$libname.la
			__runcmd sed -e 's/^installed=yes$/installed=no/' \
				     -e '/^dependency_libs=/s,${WORKDIR}[[:alnum:]/\._+-]*/\([[:alnum:]\._+-]*\),${STAGING_LIBDIR}/\1,g' \
				     -e "/^dependency_libs=/s,\([[:space:]']\)${libdir},\1${STAGING_LIBDIR},g" \
				     $dotlai >$destpath/$libname.la
		else
			rm -f $destpath/$libname.la
			__runcmd install -m 0644 $dotlai $destpath/$libname.la
		fi
	fi

	for name in $library_names; do
		files=`eval echo $name`
		for f in $files; do
			if [ ! -e "$f" ]; then
				if [ -n "$libtool" ]; then
					oefatal "oe_libinstall: $dir/$f not found."
				fi
			elif [ -L "$f" ]; then
				__runcmd cp -P "$f" $destpath/
			elif [ ! -L "$f" ]; then
				libfile="$f"
				rm -f $destpath/$libfile
				__runcmd install -m 0755 $libfile $destpath/
			fi
		done
	done

	if [ -z "$libfile" ]; then
		if  [ -n "$require_shared" ]; then
			oefatal "oe_libinstall: unable to locate shared library"
		fi
	elif [ -z "$libtool" ]; then
		# special case hack for non-libtool .so.#.#.# links
		baselibfile=`basename "$libfile"`
		if (echo $baselibfile | grep -qE '^lib.*\.so\.[0-9.]*$'); then
			sonamelink=`${HOST_PREFIX}readelf -d $libfile |grep 'Library soname:' |sed -e 's/.*\[\(.*\)\].*/\1/'`
			solink=`echo $baselibfile | sed -e 's/\.so\..*/.so/'`
			if [ -n "$sonamelink" -a x"$baselibfile" != x"$sonamelink" ]; then
				__runcmd ln -sf $baselibfile $destpath/$sonamelink
			fi
			__runcmd ln -sf $baselibfile $destpath/$solink
		fi
	fi

	__runcmd cd "$olddir"
}

oe_machinstall() {
	# Purpose: Install machine dependent files, if available
	#          If not available, check if there is a default
	#          If no default, just touch the destination
	# Example:
	#                $1  $2   $3         $4
	# oe_machinstall -m 0644 fstab ${D}/etc/fstab
	#
	# TODO: Check argument number?
	#
	filename=`basename $3`
	dirname=`dirname $3`

	for o in `echo ${OVERRIDES} | tr ':' ' '`; do
		if [ -e $dirname/$o/$filename ]; then
			oenote $dirname/$o/$filename present, installing to $4
			install $1 $2 $dirname/$o/$filename $4
			return
		fi
	done
#	oenote overrides specific file NOT present, trying default=$3...
	if [ -e $3 ]; then
		oenote $3 present, installing to $4
		install $1 $2 $3 $4
	else
		oenote $3 NOT present, touching empty $4
		touch $4
	fi
}

create_wrapper () {
   # Create a wrapper script
   #
   # These are useful to work around relocation issues, by setting environment
   # variables which point to paths in the filesystem.
   #
   # Usage: create_wrapper FILENAME [[VAR=VALUE]..]

   cmd=$1
   shift

   # run echo via env to test syntactic validity of the variable arguments
   env $@ echo "Generating wrapper script for $cmd"

   mv $cmd $cmd.real
   cmdname=`basename $cmd`.real
   cat <<END >$cmd
#!/bin/sh
exec env $@ \`dirname \$0\`/$cmdname "\$@"
END
   chmod +x $cmd
}

def check_app_exists(app, d):
	from bb import which, data

	app = data.expand(app, d)
	path = data.getVar('PATH', d, 1)
	return bool(which(path, app))

def explode_deps(s):
	return bb.utils.explode_deps(s)

def base_set_filespath(path, d):
	bb.note("base_set_filespath usage is deprecated, %s should be fixed" % d.getVar("P", 1))
	filespath = []
	# The ":" ensures we have an 'empty' override
	overrides = (bb.data.getVar("OVERRIDES", d, 1) or "") + ":"
	for p in path:
		for o in overrides.split(":"):
			filespath.append(os.path.join(p, o))
	return ":".join(filespath)
