#/bin/sh

# ------------ OpenCV Installation Script ------------
# Retrieves and installs all required dependencies for OpenCV and
# downloads and installs OpenCV libraries

FFMPEG_REPO="http://ppa.launchpad.net/jon-serverinsson/ffmpeg/ubuntu"
OPENCV_ADDR="http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/2.3.1/OpenCV-2.3.1a.tar.bz2"

# obtain system codename to install appropriate packages
SYSTEM_CODENAME=`lsb_release -sc`

if [ `lsb_release -si` = "Ubuntu" ]; then
	echo "Adding repository: $FFMPEG_REPO"
	`sudo apt-add-repository -y \'deb $FFMPEG_REPO $SYSTEM_CODENAME main\'`

	echo "Updating repository cache"
	`sudo apt-get update`
else
	echo "Skipping: Adding repository \"$FFMPEG_REPO\", because it is intended for Ubuntu systems"
fi

echo "Installing build requirements and libraries"
`sudo apt-get install -y build-essential checkinstall git cmake libfaac-dev \
libjack-jackd2-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev \
libsdl1.2-dev libtheora-dev libva-dev libvdpau-dev libvorbis-dev libx11-dev \
libxfixes-dev libxvidcore-dev texi2html yasm zlib1g-dev`

echo "Installing video libraries"
`sudo apt-get install -y libgstreamer0.10-0 libgstreamer0.10-dev gstreamer0.10-tools \
gstreamer0.10-plugins-base libgstreamer-plugins-base0.10-dev gstreamer0.10-plugins-good \
gstreamer0.10-plugins-ugly gstreamer0.10-plugins-bad gstreamer0.10-ffmpeg`

echo "Retrieving OpenCV from \"$OPENCV_ADDR\""

# install curl if not installed on system
if [ !`command -v curl` ]; then
	`sudo apt-get -y install curl`
fi
`curl -# -O "http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/2.3.1/OpenCV-2.3.1a.tar.bz2"`
