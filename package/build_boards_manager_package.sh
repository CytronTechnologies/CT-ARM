#!/bin/bash
#

ver=$1

# Figure out how will the package be called
package_name=cytron-arm-$ver
echo "Version: $ver"
echo "Package name: $package_name"

PKG_URL=https://raw.githubusercontent.com/CytronTechnologies/Cytron-ARM-Arduino-URL/master/$package_name.tar.bz2
DOC_URL=http://forum.cytron.com.my/

pushd ..

# Create directory for the package
outdir=package/versions/$ver/$package_name
srcdir=$PWD
rm -rf package/versions/$ver
mkdir -p $outdir

# Some files should be excluded from the package
cat << EOF > exclude.txt
.git
.gitignore
.travis.yml
exclude.txt
package
EOF
rsync -a --exclude-from 'exclude.txt' $srcdir/ $outdir/
rm exclude.txt

pushd package/versions/$ver
echo "Making $package_name.zip"
tar -cjvf $package_name.tar.bz2 $package_name
rm -rf $package_name

# Calculate SHA sum and size
sha=`shasum -a 256 $package_name.tar.bz2 | cut -f 1 -d ' '`
size=`/bin/ls -l $package_name.tar.bz2 | awk '{print $5}'`
echo Size: $size
echo SHA-256: $sha

echo "Making package_cytron_arm_index.json"
cat $srcdir/package/package_cytron_arm_index.template.json | \
jq ".packages[0].platforms[0].version = \"$ver\" | \
    .packages[0].platforms[0].url = \"$PKG_URL\" |\
    .packages[0].platforms[0].archiveFileName = \"$package_name.tar.bz2\" |\
    .packages[0].platforms[0].checksum = \"SHA-256:$sha\" |\
    .packages[0].platforms[0].size = \"$size\" |\
    .packages[0].platforms[0].help.online = \"$DOC_URL\"" \
    > package_cytron_arm_index.json

popd
popd
