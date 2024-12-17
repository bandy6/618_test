#!/bin/bash

die() {
	echo -e "\033[31m ERROR: $@ \033[0m"
	exit 1
}


build_dir=build
# 判定编译文件是否存在
if [ ! -d "${build_dir}" ]; then
  mkdir ${build_dir}
fi


# 进入编译目录
cd ${build_dir}

# 执行cmake
cmake ../ || die "cmake build is error!"

# 利用make进行编译
make -j8 || die "make build is error!"

# 执行安装
make install || die "make install is error!"

echo "Done."