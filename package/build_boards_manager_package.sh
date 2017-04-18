#!/bin/bash
#

# Figure out how will the package be called
ver=`git describe --tags --always`
package_name=cytron-arm-$ver
echo "Version: $ver"
echo "Package name: $package_name"

JSON_URL=https://raw.githubusercontent.com/$JSON_REPO/master/package_cytron_arm_index.json

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

# Download latest package_cytron_arm_index.json
old_json=package_cytron_arm_index_stable.json 
for i in $(seq 1 5); do
    echo "Downloading old package, try $i"
    curl -L -o $old_json $JSON_URL && break
done

#previous_nuvodude_ver=`cat $old_json | jq ".packages[0].platforms[0].toolsDependencies[1].version" | cut -d '"' -f 2`
#echo $previous_nuvodude_ver

new_json=package_cytron_arm_index.json

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
nuvodude_ver=`cat "$srcdir/package/package_cytron_arm_index.template.json" | jq ".packages[0].platforms[0].toolsDependencies[1].version" | cut -d '"' -f 2`

# if there is no release or repo migration, fall back to previous version
if [ $nuvodude_ver == null ];then
	#nuvodude_ver=$previous_nuvodude_ver
	nuvodude_ver=1.0.0
	echo Invalid repo, fall back to version $nuvodude_ver
else
	echo Version is $nuvodude_ver
fi

if [ "$nuvodude_ver" != 1.0.0 ];then

	download_url=https://github.com/$NUVODUDE_REPO/releases/download/$nuvodude_ver
	nuvodude_win32=nuvodude-$nuvodude_ver-win32.zip
	nuvodude_osx=nuvodude-$nuvodude_ver-osx.tar.gz
	nuvodude_linux32=nuvodude-$nuvodude_ver-linux32.tar.gz
	nuvodude_linux64=nuvodude-$nuvodude_ver-linux64.tar.gz

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

        echo "Building json file"
	cat tmp | \
	jq ".packages[0].tools[1].version = \"$nuvodude_ver\"|\
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
            .packages[0].tools[1].systems[3].size = \"$nuvodude_linux32_size\"" \
	> $new_json
	rm $nuvodude_win32 && rm $nuvodude_osx && rm $nuvodude_linux64 && rm $nuvodude_linux32
else
	cat tmp > $new_json
fi

set +e
python ../../merge_packages.py $new_json $old_json >tmp && mv tmp $new_json && rm $old_json

# deploy key
echo -n $CT_ARM_DEPLOY_KEY > ~/.ssh/ct_arm_deploy_b64
base64 --decode --ignore-garbage ~/.ssh/ct_arm_deploy_b64 > ~/.ssh/ct_arm_deploy
chmod 600 ~/.ssh/ct_arm_deploy
echo -e "Host $DEPLOY_HOST_NAME\n\tHostname github.com\n\tUser $DEPLOY_USER_NAME\n\tStrictHostKeyChecking no\n\tIdentityFile ~/.ssh/ct_arm_deploy" >> ~/.ssh/config

#update package_cytron_arm_index.json
git clone $DEPLOY_USER_NAME@$DEPLOY_HOST_NAME:$JSON_REPO.git ~/tmp
cp $new_json ~/tmp/

popd
popd
