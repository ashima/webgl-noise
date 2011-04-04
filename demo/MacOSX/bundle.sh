#!/bin/sh

# Creates application bundles for use on Mac OS X.

if [ -z "$1" ]; then
  echo "usage: `basename $0` BUNDLE-NAME exec-name"
  exit 1
fi

bundle_name="$1"
exec_file="$2"
exec_name=`basename $bundle_name .app`

if [ ! -f $exec_file ]; then
  echo "Can't find $exec_file"
  exit 1
fi

if [ ! -d "${bundle_name}/Contents/MacOS" ]; then
  mkdir -p "${bundle_name}/Contents/MacOS"
fi

cp $exec_file "${bundle_name}/Contents/MacOS/" 

touch "${bundle_name}"

if [ ! -d "${bundle_name}/Contents/Resources" ]; then
  mkdir -p "${bundle_name}/Contents/Resources"
fi

if [ ! -f "${bundle_name}/Contents/PkgInfo" ]; then
  echo -n "APPL????" > "${bundle_name}/Contents/PkgInfo"
fi

if [ ! -f "${bundle_name}/Contents/Info.plist" ]; then
  cat > "${bundle_name}/Contents/Info.plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>CFBundleName</key>        <string>${exec_name}</string>
  <key>CFBundleExecutable</key>  <string>${exec_name}</string>
  <key>CFBundleIdentifier</key>  <string>com.ashimaarts.${exec_name}</string>
  <key>CFBundleVersion</key>     <string>0.1</string>

  <key>CFBundlePackageType</key>           <string>APPL</string>
  <key>CFBundleDevelopmentRegion</key>     <string>English</string>
  <key>CFBundleSignature</key>             <string>????</string>
  <key>CFBundleInfoDictionaryVersion</key> <string>6.0</string>
</dict>
</plist>
EOF
fi
