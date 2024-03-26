# download

这里下载的Android版本是android-6.0.1_r46，其它版本操作类似。执行下面命令：

在你的home下创建bin目录，并加入PATH

``` bash
mkdir -p ~/.bin
PATH="${HOME}/.bin:${PATH}"
git clone https://github.com/GerritCodeReview/git-repo.git  ./bin/repo
# chmod a+rx ~/.bin/repo/repo
export PATH=~/.bin/repo/:$PATH
```

## 创建一个空目录用来保存android源代码

``` bash
mkdir ~/aosp
cd ~/aosp
```

## 指定从清华的镜像，只下6.0.1版本Android

``` bash
repo init -u https://aosp.tuna.tsinghua.edu.cn/platform/manifest -b android-6.0.1_r46
#同步源码树，并发设置为8，不要太大
repo sync -j8
```

下载的话大概2-3个小时即可完成。

源代码下载完成后，开始编译，实测35分钟完成。

初始化环境：

``` bash
source build/envsetup.sh
```

## 设置环境变量，不然编译会报错

``` bash
export USER=$(whoami)
```

选择一个编译的target（我想刷到Nexus5上去）：

lunch aosp_hammerhead-eng

执行lunch会弹出选择 lunch后面的参数是BUILD-BUILDTYPE。BUILDTYPE有3种：

user， limited access; suited for production
userdebug， like user but with root access and debuggability; preferred for debugging
eng ，development configuration with additional debugging tools

最后开始编译，j参数后设置并发数，一般设置为核数*2：

make -j16

编译完成。x
