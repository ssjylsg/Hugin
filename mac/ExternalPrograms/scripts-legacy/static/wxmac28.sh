# ------------------
#     wxMac 2.8
# ------------------
# $Id: wxmac28.sh 1902 2007-02-04 22:27:47Z ippei $
# Copyright (c) 2007-2008, Ippei Ukai


# prepare

# export REPOSITORYDIR="/PATH2HUGIN/mac/ExternalPrograms/repository" \
#  ARCHS="ppc i386" \
#  ppcTARGET="powerpc-apple-darwin8" \
#  ppcOSVERSION="10.4" \
#  ppcMACSDKDIR="/Developer/SDKs/MacOSX10.4u.sdk" \
#  ppcOPTIMIZE="-mcpu=G3 -mtune=G4" \
#  i386TARGET="i386-apple-darwin8" \
#  i386OSVERSION="10.4" \
#  i386MACSDKDIR="/Developer/SDKs/MacOSX10.4u.sdk" \
#  i386OPTIMIZE ="-march=prescott -mtune=pentium-m -ftree-vectorize" \
#  OTHERARGs="";

# -------------------------------
# 20091206.0 sg Script NOT tested but follows logic of dynamic wxmac28.sh
#               Works Intel: 10.5, 10.6 & Powerpc 10.4, 10.5
# -------------------------------

uname_release=$(uname -r)
uname_arch=$(uname -p)
[ $uname_arch = powerpc ] && uname_arch="ppc"
os_dotvsn=${uname_release%%.*}
os_dotvsn=$(($os_dotvsn - 4))
case $os_dotvsn in
 4 ) os_sdkvsn="10.4u" ;;
 5|6 ) os_sdkvsn=10.$os_dotvsn ;;
 * ) echo "Unhandled OS Version: 10.$os_dotvsn. Build aborted."; exit 1 ;;
esac

NATIVE_SDKDIR="/Developer/SDKs/MacOSX$os_sdkvsn.sdk"
NATIVE_OSVERSION="10.$os_dotvsn"
NATIVE_ARCH=$uname_arch
NATIVE_OPTIMIZE=""

# patch for Snow Leopard
thisarch=$(uname -m)
if [ "$thisarch" = x86_64 ] ; then
	patch -Np1 < ../scripts/wxMac-2.8.10.patch
fi

WXVERSION="2.8"

mkdir -p "$REPOSITORYDIR/bin";
mkdir -p "$REPOSITORYDIR/lib";
mkdir -p "$REPOSITORYDIR/include";

# compile

# remove 64-bit archs from ARCHS
ARCHS_TMP=$ARCHS
ARCHS=""
let NUMARCH="0"

for ARCH in $ARCHS_TMP
do
  if [ $ARCH = "i386" -o $ARCH = "i686" -o $ARCH = "ppc" -o $ARCH = "ppc750" -o $ARCH = "ppc7400" ]
  then
    NUMARCH=$(($NUMARCH + 1))
    if [ -n $ARCHS ] ; then
      ARCHS="$ARCHS $ARCH"
    else
      ARCHS=$ARCH
    fi
  fi
done


for ARCH in $ARCHS
do

  mkdir -p "osx-$ARCH-build";
  cd "osx-$ARCH-build";

  mkdir -p "$REPOSITORYDIR/arch/$ARCH/bin";
  mkdir -p "$REPOSITORYDIR/arch/$ARCH/lib";
  mkdir -p "$REPOSITORYDIR/arch/$ARCH/include";

  ARCHARGs=""
  MACSDKDIR=""

 if [ $ARCH = "i386" -o $ARCH = "i686" ] ; then
   TARGET=$i386TARGET
   MACSDKDIR=$i386MACSDKDIR
   ARCHARGs="$i386ONLYARG"
   OSVERSION="$i386OSVERSION"
   CC=$i386CC
   CXX=$i386CXX
 elif [ $ARCH = "ppc" -o $ARCH = "ppc750" -o $ARCH = "ppc7400" ] ; then
   TARGET=$ppcTARGET
   MACSDKDIR=$ppcMACSDKDIR
   ARCHARGs="$ppcONLYARG"
   OSVERSION="$ppcOSVERSION"
   CC=$ppcCC
   CXX=$ppcCXX
  fi
 
 env \
   CC=$CC CXX=$CXX \
   CFLAGS="-arch $ARCH $ARCHARGs $OTHERARGs -O2 -dead_strip" \
   CXXFLAGS="-arch $ARCH $ARCHARGs $OTHERARGs -O2 -dead_strip" \
   CPPFLAGS="-I$REPOSITORYDIR/include" \
   OBJCFLAGS="-arch $ARCH" \
   OBJCXXFLAGS="-arch $ARCH" \
   LDFLAGS="-L$REPOSITORYDIR/lib -mmacosx-version-min=$OSVERSION -dead_strip -prebind" \
   ../configure --prefix="$REPOSITORYDIR" --exec-prefix=$REPOSITORYDIR/arch/$ARCH --disable-dependency-tracking \
     --host="$TARGET" --with-macosx-sdk=$MACSDKDIR --with-macosx-version-min=$OSVERSION \
     --enable-monolithic --enable-unicode --with-opengl --enable-compat26 --disable-graphics_ctx \
     --disable-shared --disable-debug;


# disable core graphics implementation for 10.3
# need to find out where setup.h was created. This seems to vary if building on powerpc and
# is different under 10.4 and 10.5
 whereIsSetup=$(find . -name setup.h -print)
 whereIsSetup=${whereIsSetup#./}

 echo '#ifndef wxMAC_USE_CORE_GRAPHICS'    >> $whereIsSetup
 echo ' #define wxMAC_USE_CORE_GRAPHICS 0' >> $whereIsSetup
 echo '#endif'                             >> $whereIsSetup
 echo ''                                   >> $whereIsSetup

 make clean;

#hack
 cp utils/wxrc/Makefile utils/wxrc/Makefile-copy;
 echo "all: " > utils/wxrc/Makefile;
 echo "" >> utils/wxrc/Makefile;
 echo "install: " >> utils/wxrc/Makefile;
#~hack

case $NATIVE_OSVERSION in
	 10.4 )
    dylib_name="dylib1.o"
		 ;;
	 10.5 | 10.6 )
		 dylib_name="dylib1.10.5.o"
		 ;;
	 * )
		 echo "OS Version $NATIVE_OSVERSION not supported"; exit 1
		 ;;
esac
cp $NATIVE_SDKDIR/usr/lib/$dylib_name $REPOSITORYDIR/lib/

# Need to build single-threaded. libwx_macu-2.8.dylib needs to be built before libwx_macu_gl-2.8 to avoid a link error.
# This is only problematic for Intel builds, where jobs can be >1
 make --jobs=1;
 make install;

 rm $REPOSITORYDIR/lib/$dylib_name;

 cd ../;

done


# merge libwx

for libname in lib/libwx_macu-$WXVERSION lib/libwx_macu_gl-$WXVERSION lib/libwxregexu-$WXVERSION
do

   liba="$libname.a"

  if [ $NUMARCH -eq 1 ] ; then
   if [ -f $REPOSITORYDIR/arch/$ARCHS/$liba ] ; then
		 echo "Moving arch/$ARCHS/$liba to $liba"
  	 mv "$REPOSITORYDIR/arch/$ARCHS/$liba" "$REPOSITORYDIR/$liba";
	   #Power programming: if filename ends in "a" then ...
	   [ ${liba##*.} = a ] && ranlib "$REPOSITORYDIR/$liba";
  	 continue
	 else
		 echo "Program arch/$ARCHS/$liba not found. Aborting build";
		 exit 1;
	 fi
  fi

   LIPOARGs=""

  for ARCH in $ARCHS
  do
	if [ -f $REPOSITORYDIR/arch/$ARCH/$liba ] ; then
		echo "Adding arch/$ARCH/$liba to bundle"
		LIPOARGs="$LIPOARGs $REPOSITORYDIR/arch/$ARCH/$liba"
	else
		echo "File arch/$ARCH/$liba was not found. Aborting build";
		exit 1;
	fi
  done

  lipo $LIPOARGs -create -output "$REPOSITORYDIR/$libname.a";
  ranlib "$REPOSITORYDIR/$libname.a";

done

# merge setup.h

for confname in "wx/setup.h"
do

  wxmacconf="lib/wx/include/mac-unicode-release-static-$WXVERSION/$confname"
  mkdir -p $(dirname $REPOSITORYDIR/$wxmacconf);
  echo "" > "$REPOSITORYDIR/$wxmacconf";

 if [ $NUMARCH -eq 1 ] ; then
   ARCH=$ARCHS
   pushd $REPOSITORYDIR
	 whereIsSetup=$(find ./arch/$ARCH/lib/wx -name setup.h -print)
	 whereIsSetup=${whereIsSetup#./arch/*/}
	 popd 
	 cat "$REPOSITORYDIR/arch/$ARCH/$whereIsSetup" >>"$REPOSITORYDIR/$wxmacconf";
   continue
 fi

  for ARCH in $ARCHS
  do
	  pushd $REPOSITORYDIR
	  whereIsSetup=$(find ./arch/$ARCH/lib/wx -name setup.h -print)
	  whereIsSetup=${whereIsSetup#./arch/*/}
	  popd 

	   if [ $ARCH = "i386" -o $ARCH = "i686" ] ; then
	     echo "#if defined(__i386__)"                       >> "$REPOSITORYDIR/$wxmacconf";
	     echo ""                                            >> "$REPOSITORYDIR/$wxmacconf";
			 cat "$REPOSITORYDIR/arch/$ARCH/$whereIsSetup"      >> "$REPOSITORYDIR/$wxmacconf";
	     echo ""                                            >> "$REPOSITORYDIR/$wxmacconf";
	     echo "#endif"                                      >> "$REPOSITORYDIR/$wxmacconf";
	   elif [ $ARCH = "ppc" -o $ARCH = "ppc750" -o $ARCH = "ppc7400" ] ; then
	     echo "#if defined(__ppc__) || defined(__ppc64__)"  >> "$REPOSITORYDIR/$wxmacconf";
	     echo ""                                            >> "$REPOSITORYDIR/$wxmacconf";
			 cat "$REPOSITORYDIR/arch/$ARCH/$whereIsSetup"      >> "$REPOSITORYDIR/$wxmacconf";
	     echo ""                                            >> "$REPOSITORYDIR/$wxmacconf";
	     echo "#endif"                                      >> "$REPOSITORYDIR/$wxmacconf";
	  else
			 echo "Unhandled ARCH: $ARCH. Aborting build."; exit 1
	   fi
  done
done
