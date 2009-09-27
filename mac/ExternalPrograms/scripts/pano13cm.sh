# ------------------
#     pano13cm
# ------------------
# $Id: pano13cm.sh 1904 2007-02-05 00:10:54Z ippei $
# Copyright skeleton (c) 2007, Ippei Ukai
# script modifications and cmake, Harry van der Wolf

# prepare

# export REPOSITORYDIR="/PATH2HUGIN/mac/ExternalPrograms/repository" \
# ARCHS="ppc i386" \
#  ppcTARGET="powerpc-apple-darwin8" \
#  i386TARGET="i386-apple-darwin8" \
#  ppcMACSDKDIR="/Developer/SDKs/MacOSX10.4u.sdk" \
#  i386MACSDKDIR="/Developer/SDKs/MacOSX10.4u.sdk" \
#  ppcONLYARG="-mcpu=G3 -mtune=G4" \
#  i386ONLYARG="-mfpmath=sse -msse2 -mtune=pentium-m -ftree-vectorize" \
#  ppc64ONLYARG="-mcpu=G5 -mtune=G5 -ftree-vectorize" \
#  OTHERARGs="";
  
# init

let NUMARCH="0"

for i in $ARCHS
do
  NUMARCH=$(($NUMARCH + 1))
done

mkdir -p "$REPOSITORYDIR/bin";
mkdir -p "$REPOSITORYDIR/lib";
mkdir -p "$REPOSITORYDIR/include";


# compile

for ARCH in $ARCHS
do

 mkdir -p "$REPOSITORYDIR/arch/$ARCH/bin";
 mkdir -p "$REPOSITORYDIR/arch/$ARCH/lib";
 mkdir -p "$REPOSITORYDIR/arch/$ARCH/include";

 ARCHARGs=""
 MACSDKDIR=""

 if [ $ARCH = "i386" -o $ARCH = "i686" ]
 then
  TARGET=$i386TARGET
  MACSDKDIR=$i386MACSDKDIR
  ARCHARGs="$i386ONLYARG"
  OSVERSION=$i386OSVERSION
  OPTIMIZE=$i386OPTIMIZE
  export CC=$I386CC;
  export CXX=$I386CXX;
 elif [ $ARCH = "ppc" -o $ARCH = "ppc750" -o $ARCH = "ppc7400" ]
 then
  TARGET=$ppcTARGET
  MACSDKDIR=$ppcMACSDKDIR
  ARCHARGs="$ppcONLYARG"
  OSVERSION=$ppcOSVERSION
  OPTIMIZE=$ppcOPTIMIZE
  export CC=$ppcCC;
  export CXX=$ppcCXX;
 elif [ $ARCH = "ppc64" -o $ARCH = "ppc970" ]
 then
  TARGET=$ppc64TARGET
  MACSDKDIR=$ppc64MACSDKDIR
  ARCHARGs="$ppc64ONLYARG"
  OSVERSION=$ppc64OSVERSION
  OPTIMIZE=$ppc64OPTIMIZE
  export CC=$ppc64CC;
  export CXX=$ppc64CXX;
 elif [ $ARCH = "x86_64" ]
 then
  TARGET=$x64TARGET
  MACSDKDIR=$x64MACSDKDIR
  ARCHARGs="$x64ONLYARG"
  OSVERSION=$x64OSVERSION
  OPTIMIZE=$x64OPTIMIZE
  export CC=$x64CC;
  export CXX=$x64CXX;
 fi
 
 mkdir $ARCH;
 cd $ARCH;
 rm CMakeCache.txt;

 env CFLAGS="-isysroot $MACSDKDIR -arch $ARCH $ARCHARGs $OTHERARGs -O2 -dead_strip" \
  CXXFLAGS="-isysroot $MACSDKDIR -arch $ARCH $ARCHARGs $OTHERARGs -O2 -dead_strip" \
  CPPFLAGS="-I$REPOSITORYDIR/include" \
  LDFLAGS="-L$REPOSITORYDIR/lib -dead_strip -prebind" \
  NEXT_ROOT="$MACSDKDIR" \
  PKG_CONFIG_PATH="$REPOSITORYDIR/lib/pkgconfig" ;

 cmake \
  -DCMAKE_VERBOSE_MAKEFILE:BOOL="ON" \
  -DCMAKE_INSTALL_PREFIX:PATH="$REPOSITORYDIR/arch/$ARCH" \
  -DCMAKE_BUILD_TYPE:STRING="Release" \
  -DCMAKE_C_FLAGS_RELEASE:STRING="-arch $ARCH -mmacosx-version-min=$OSVERSION -isysroot $MACSDKDIR -DNDEBUG -O2 $OPTIMIZE" \
  -DCMAKE_CXX_FLAGS_RELEASE:STRING="-arch $ARCH -mmacosx-version-min=$OSVERSION -isysroot $MACSDKDIR -DNDEBUG -O2 $OPTIMIZE" \
  -DJPEG_INCLUDE_DIR="$REPOSITORYDIR/include" \
  -DJPEG_LIBRARIES="$REPOSITORYDIR/lib/libjpeg.dylib" \
  -DPNG_INCLUDE_DIR="$REPOSITORYDIR/include" \
  -DPNG_LIBRARIES="$REPOSITORYDIR/lib/libpng.dylib" \
  -DTIFF_INCLUDE_DIR="$REPOSITORYDIR/include" \
  -DTIFF_LIBRARIES="$REPOSITORYDIR/lib/libtiff.dylib" \
  -DZLIB_INCLUDE_DIR="/usr/include" \
  -DZLIB_LIBRARIES="/usr/lib/libz.dylib" \
  ..;

 make;
 make install;
 
 cd ..
done


# merge libpano13

GENERATED_DYLIB_NAME="libpano13.2.0.0.dylib";
GENERATED_DYLIB_INSTALL_NAME="libpano13.2.dylib";

for liba in lib/libpano13.a lib/$GENERATED_DYLIB_NAME
do

 if [ $NUMARCH -eq 1 ]
 then
  mv "$REPOSITORYDIR/arch/$ARCHS/$liba" "$REPOSITORYDIR/$liba";
  if [[ $liba == *.a ]]
  then 
   ranlib "$REPOSITORYDIR/$liba";
  fi
  continue
 fi

 LIPOARGs=""
 
 for ARCH in $ARCHS
 do
  LIPOARGs="$LIPOARGs $REPOSITORYDIR/arch/$ARCH/$liba"
 done

 lipo $LIPOARGs -create -output "$REPOSITORYDIR/$liba";
 if [[ $liba == *.a ]]
 then 
  ranlib "$REPOSITORYDIR/$liba";
 fi

done

mv $REPOSITORYDIR/lib/$GENERATED_DYLIB_NAME $REPOSITORYDIR/lib/libpano13.dylib;

for libname in pano13
do
 if [ -f "$REPOSITORYDIR/lib/lib$libname.dylib" ]
 then
  install_name_tool -id "$REPOSITORYDIR/lib/lib$libname.dylib" "$REPOSITORYDIR/lib/lib$libname.dylib";
 fi
done


# merge execs

for program in bin/panoinfo bin/PTblender bin/PTcrop bin/PTinfo bin/PTmasker bin/PTmender bin/PToptimizer bin/PTroller bin/PTtiff2psd bin/PTtiffdump bin/PTuncrop
do

 LIPOARGs=""

 if [ $NUMARCH -eq 1 ]
 then
  mv "$REPOSITORYDIR/arch/$ARCHS/$program" "$REPOSITORYDIR/$program";
  install_name_tool \
    -change "$REPOSITORYDIR/arch/$ARCHS/lib/$GENERATED_DYLIB_INSTALL_NAME" "$REPOSITORYDIR/lib/libpano13.dylib" \
    "$REPOSITORYDIR/$program";
  continue
 fi

 for ARCH in $ARCHS
 do
  LIPOARGs="$LIPOARGs $REPOSITORYDIR/arch/$ARCH/$program"
 done

 lipo $LIPOARGs -create -output "$REPOSITORYDIR/$program";
 
 for ARCH in $ARCHS
 do
  install_name_tool \
    -change "$REPOSITORYDIR/arch/$ARCH/lib/$GENERATED_DYLIB_INSTALL_NAME" "$REPOSITORYDIR/lib/libpano13.dylib" \
    "$REPOSITORYDIR/$program";
 done

done

