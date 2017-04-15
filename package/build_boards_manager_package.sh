#!/bin/bash
#

# Figure out how will the package be called
ver=`git describe --tags --always`
package_name=cytron-arm-$ver
echo "Version: $ver"
echo "Package name: $package_name"

CT_ARM_URL_REPO=CytronTechnologies/CT-ARM
NUVODUDE_REPO=CytronTechnologies/nuvodude

PKG_URL=https://github.com/$CT_ARM_URL_REPO/releases/download/$ver/$package_name.zip
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
docs
EOF
rsync -a --exclude-from 'exclude.txt' $srcdir/ $outdir/
rm exclude.txt

pushd package/versions/$ver
echo "Making $package_name.zip"
zip -qr $package_name.zip $package_name
rm -rf $package_name

# Calculate SHA sum and size
sha=`shasum -a 256 $package_name.zip | cut -f 1 -d ' '`
size=`/bin/ls -l $package_name.zip | awk '{print $5}'`
echo Size: $size
echo SHA-256: $sha

# get latest nuvodude info
echo "Getting nuvodude latest version"
nuvodude_ver=`curl -s https://api.github.com/repos/$NUVODUDE_REPO/releases/latest | jq -r '.tag_name'`
echo $nuvodude_ver

# Download latest package_cytron_arm_index.json
old_json=package_cytron_arm_index_stable.json 
for i in $(seq 1 5); do
    echo "Downloading old package, try $i"
    curl -L -o $old_json https://raw.githubusercontent.com/CytronTechnologies/Cytron-ARM-Arduino-URL/master/package_cytron_arm_index.json && break
done

new_json=package_cytron_arm_index.json

echo "Making package_cytron_arm_index.json"
cat $srcdir/package/package_cytron_arm_index.template.json | \
jq ".packages[0].platforms[0].version = \"$ver\" | \
    .packages[0].platforms[0].url = \"$PKG_URL\" |\
    .packages[0].platforms[0].archiveFileName = \"$package_name.zip\" |\
    .packages[0].platforms[0].checksum = \"SHA-256:$sha\" |\
    .packages[0].platforms[0].size = \"$size\" |\
    .packages[0].platforms[0].help.online = \"$DOC_URL\"" \
    > $new_json

set +e
python ../../merge_packages.py $new_json $old_json >tmp && mv tmp $new_json && rm $old_json

popd
popd
