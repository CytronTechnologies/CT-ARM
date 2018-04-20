#!/bin/bash

# Figure out how will the package be called
ver=`git describe --tags --always`
package_name=cytron-arm-$ver
echo "Version: $ver"
echo "Package name: $package_name"

if [ -z "$HOST_URL" ]; then
	HOST_URL=bengchet.github.io
fi

if [ -z "$ORGANISATION" ]; then
	ORGANISATION=bengchet
fi

JSON_URL=https://$HOST_URL/package_cytron_index.json
PKG_URL=https://github.com/$ORGANISATION/CT-ARM/releases/download/$ver/$package_name.zip
DOC_URL=https://$HOST_URL/CT-ARM/

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
mkdocs.yml
exclude.txt
package
tools
docs
EOF
# Also include all files which are ignored by git
git ls-files --other --directory >> exclude.txt
rsync -a --exclude-from 'exclude.txt' $srcdir/ $outdir/
rm exclude.txt

# For compatibility, on OS X we need GNU sed which is usually called 'gsed'
if [ "$(uname)" == "Darwin" ]; then
    SED=gsed
else
    SED=sed
fi

# Do some replacements in platform.txt file, which are required because IDE
# handles tool paths differently when package is installed in hardware folder
cat $srcdir/platform.txt | \
$SED 's/runtime.tools.arm-none-eabi-gcc.path={runtime.platform.path}\/tools\/gcc-arm-none-eabi//g' | \
$SED 's/tools.nulinkburn.path={runtime.platform.path}\/tools\/nuvodude/tools.nulinkburn.path=\{runtime.tools.nulinkburn.path\}/g' | \
$SED 's/version=.*/version='$ver'/g' \
 > $outdir/platform.txt


pushd package/versions/$ver
echo "Making $package_name.zip"
zip -qr $package_name.zip $package_name
rm -rf $package_name

# Calculate SHA sum and size
sha=`shasum -a 256 $package_name.zip | cut -f 1 -d ' '`
size=`/bin/ls -l $package_name.zip | awk '{print $5}'`
echo Size: $size
echo SHA-256: $sha

# Download latest package_cytron_index.json
old_json=package_cytron_index_stable.json
for i in $(seq 1 5); do
    echo "Downloading old package, try $i"
    curl -L -o $old_json $JSON_URL && break
done

if [ ! -e package_cytron_index_stable.json ]; then
cat $srcdir/package/package_cytron_arm_index.template.json > $old_json
fi

new_json=package_cytron_index.json

echo "Making package_cytron_arm_index.json"
cat $srcdir/package/package_cytron_arm_index.template.json | \
jq ".packages[0].platforms[0].version = \"$ver\" | \
    .packages[0].platforms[0].url = \"$PKG_URL\" |\
    .packages[0].platforms[0].archiveFileName = \"$package_name.zip\" |\
    .packages[0].platforms[0].checksum = \"SHA-256:$sha\" |\
    .packages[0].platforms[0].size = \"$size\" |\
    .packages[0].platforms[0].help.online = \"$DOC_URL\"" \
    > tmp

# get nuvodude version info
echo "Getting nuvodude stable version"
curl --silent https://api.github.com/repos/$ORGANISATION/nuvodude/releases > releases.json
# Previous final release (prerelase == false)
prev_release=$(jq -r '. | map(select(.draft == false and .prerelease == false)) | sort_by(.created_at | - fromdateiso8601) | .[0].tag_name' releases.json)
# Previous release (possibly a pre-release)
prev_any_release=$(jq -r '. | map(select(.draft == false)) | sort_by(.created_at | - fromdateiso8601)  | .[0].tag_name' releases.json)
# Previous pre-release
prev_pre_release=$(jq -r '. | map(select(.draft == false and .prerelease == true)) | sort_by(.created_at | - fromdateiso8601)  | .[0].tag_name' releases.json)

echo "Previous release: $prev_release"
echo "Previous (pre-?)release: $prev_any_release"
echo "Previous pre-release: $prev_pre_release"

nuvodude_ver=$prev_any_release

if [ $nuvodude_ver == null ];then
	echo "Getting nuvodude stable version from template json"
	nuvodude_ver=`cat "$srcdir/package/package_cytron_arm_index.template.json" | jq ".packages[0].platforms[0].toolsDependencies[1].version" | cut -d '"' -f 2`
fi

echo "Current Version of nuvodude is $nuvodude_ver"

if [ "$nuvodude_ver" != 1.0.0 ];then

	download_url=https://github.com/$ORGANISATION/nuvodude/releases/download/$nuvodude_ver
	nuvodude_win32=nuvodude-$nuvodude_ver-win32.zip
	nuvodude_osx=nuvodude-$nuvodude_ver-osx.tar.gz
	nuvodude_linux32=nuvodude-$nuvodude_ver-linux32.tar.gz
	nuvodude_linux64=nuvodude-$nuvodude_ver-linux64.tar.gz
	nuvodude_linux_arm=nuvodude-$nuvodude_ver-linux-armhf.tar.gz

	echo "Build nuvodude version : $nuvodude_ver"
	echo "Getting necessary files info"

	# Getting windows
	echo "Getting info from $nuvodude_win32"
	wget -q -O $nuvodude_win32 $download_url/$nuvodude_win32
	nuvodude_win32_sha=`shasum -a 256 $nuvodude_win32 | cut -f 1 -d ' '`
	nuvodude_win32_size=`/bin/ls -l $nuvodude_win32 | awk '{print $5}'`
	echo Size: $nuvodude_win32_size
	echo SHA-256: $nuvodude_win32_sha

	# Getting osx
	echo "Getting info from $nuvodude_osx"
	wget -q -O $nuvodude_osx $download_url/$nuvodude_osx
	nuvodude_osx_sha=`shasum -a 256 $nuvodude_osx | cut -f 1 -d ' '`
	nuvodude_osx_size=`/bin/ls -l $nuvodude_osx | awk '{print $5}'`
	echo Size: $nuvodude_osx_size
	echo SHA-256: $nuvodude_osx_sha

	# Getting linux64
	echo "Getting info from $nuvodude_linux64"
	wget -q -O $nuvodude_linux64 $download_url/$nuvodude_linux64
	nuvodude_linux64_sha=`shasum -a 256 $nuvodude_linux64 | cut -f 1 -d ' '`
	nuvodude_linux64_size=`/bin/ls -l $nuvodude_linux64 | awk '{print $5}'`
	echo Size: $nuvodude_linux64_size
	echo SHA-256: $nuvodude_linux64_sha

	# Getting linux32
	echo "Getting info from $nuvodude_linux32"
	wget -q -O $nuvodude_linux32 $download_url/$nuvodude_linux32
	nuvodude_linux32_sha=`shasum -a 256 $nuvodude_linux32 | cut -f 1 -d ' '`
	nuvodude_linux32_size=`/bin/ls -l $nuvodude_linux32 | awk '{print $5}'`
	echo Size: $nuvodude_linux32_size
	echo SHA-256: $nuvodude_linux32_sha
	
	# Getting linux arm
	echo "Getting info from $nuvodude_linux_arm"
	wget -q -O $nuvodude_linux_arm $download_url/$nuvodude_linux_arm
	nuvodude_linux_arm_sha=`shasum -a 256 $nuvodude_linux_arm | cut -f 1 -d ' '`
	nuvodude_linux_arm_size=`/bin/ls -l $nuvodude_linux_arm | awk '{print $5}'`
	echo Size: $nuvodude_linux_arm_size
	echo SHA-256: $nuvodude_linux_arm_sha

    echo "Building json file"
	cat tmp | \
	jq ".packages[0].platforms[0].toolsDependencies[1].version = \"$nuvodude_ver\"|\
		.packages[0].tools[1].version = \"$nuvodude_ver\"|\
	    .packages[0].tools[1].systems[0].archiveFileName = \"$nuvodude_win32\"|\
        .packages[0].tools[1].systems[0].url = \"$download_url/$nuvodude_win32\"|\
	    .packages[0].tools[1].systems[0].checksum = \"SHA-256:$nuvodude_win32_sha\"|\
        .packages[0].tools[1].systems[0].size = \"$nuvodude_win32_size\"|\
	    .packages[0].tools[1].systems[1].archiveFileName = \"$nuvodude_osx\"|\
        .packages[0].tools[1].systems[1].url = \"$download_url/$nuvodude_osx\"|\
        .packages[0].tools[1].systems[1].checksum = \"SHA-256:$nuvodude_osx_sha\"|\
        .packages[0].tools[1].systems[1].size = \"$nuvodude_osx_size\"|\
	    .packages[0].tools[1].systems[2].archiveFileName = \"$nuvodude_linux64\"|\
        .packages[0].tools[1].systems[2].url = \"$download_url/$nuvodude_linux64\"|\
        .packages[0].tools[1].systems[2].checksum = \"SHA-256:$nuvodude_linux64_sha\"|\
        .packages[0].tools[1].systems[2].size = \"$nuvodude_linux64_size\"|\
        .packages[0].tools[1].systems[3].archiveFileName = \"$nuvodude_linux32\"|\
        .packages[0].tools[1].systems[3].url = \"$download_url/$nuvodude_linux32\"|\
	    .packages[0].tools[1].systems[3].checksum = \"SHA-256:$nuvodude_linux32_sha\"|\
        .packages[0].tools[1].systems[3].size = \"$nuvodude_linux32_size\"|\
	    .packages[0].tools[1].systems[4].archiveFileName = \"$nuvodude_linux_arm\"|\
        .packages[0].tools[1].systems[4].url = \"$download_url/$nuvodude_linux_arm\"|\
	    .packages[0].tools[1].systems[4].checksum = \"SHA-256:$nuvodude_linux_arm_sha\"|\
        .packages[0].tools[1].systems[4].size = \"$nuvodude_linux_arm_size\"" \
	> $new_json
	rm $nuvodude_win32 && rm $nuvodude_osx && rm $nuvodude_linux64 && rm $nuvodude_linux32 && rm  $nuvodude_linux_arm
else
	cat tmp > $new_json
fi

set +e
python ../../merge_packages.py $new_json $old_json >tmp && mv tmp $new_json && rm $old_json

# remove releases.json
rm releases.json

# deploy key
echo -n $DEPLOY_KEY > ~/.ssh/deploy_b64
base64 --decode --ignore-garbage ~/.ssh/deploy_b64 > ~/.ssh/deploy
chmod 600 ~/.ssh/deploy
echo -e "Host $DEPLOY_HOST_NAME\n\tHostname github.com\n\tUser $DEPLOY_USER_NAME\n\tStrictHostKeyChecking no\n\tIdentityFile ~/.ssh/deploy" >> ~/.ssh/config

#update package_cytron_index.json
git clone $DEPLOY_USER_NAME@$DEPLOY_HOST_NAME:$ORGANISATION/$HOST_URL.git ~/tmp
cp $new_json ~/tmp/

popd
popd
