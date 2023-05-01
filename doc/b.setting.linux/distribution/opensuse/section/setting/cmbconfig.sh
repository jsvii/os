#!/bin/sh

## http://luoliyong%40it.com:1qaz%40WSX@git.cs
cmb_proxy="http://192.168.1.91:808"
#ext_proxy="http://192.168.1.1:1100"
ext_proxy="http://192.168.1.95:8118"

# npm registry
cmb_npm_registry="http://buildrepo.cs/artifactory/api/npm/v-npm"
cmb_disturl="http://buildrepo.cs/artifactory/r-npm-taobao-m-nodedist/"
cmb_electron_mirror="http://buildrepo.cs/artifactory/r-npm-taobao-m-electron/"
cmb_sass_binary_site="http://buildrepo.cs/artifactory/r-npm-taobao-m-nodesass"
cmb_phantomjs_cdnurl="http://buildrepo.cs/artifactory/r-npm-taobao-m-phantomjs/"
cmb_chromedriver_cdnurl="http://buildrepo.cs/artifactory/r-npm-taobao-m-chromedriver/"
cmb_operadriver_cdnurl="http://buildrepo.cs/artifactory/r-npm-taobao-m-operadriver/"
cmb_SQLITE3_BINARY_SITE="http://buildrepo.cs/artifactory/r-npm-taobao-m-sqlite3/"
cmb_profiler_binary_host_mirror="http://buildrepo.cs/artifactory/r-npm-taobao-m-nodeinspector/"

#taobao
taobao_npm_registry="http://registry.npm.taobao.org/"
taobao_disturl="https://npm.taobao.org/mirrors/node"
taobao_electron_mirror="http://buildrepo.cs/artifactory/r-npm-taobao-m-electron/"
taobao_sass_binary_site="https://npm.taobao.org/mirrors/node-sass"
taobao_phantomjs_cdnurl="http://npm.taobao.org/mirrors/phantomjs/"
taobao_chromedriver_cdnurl="http://buildrepo.cs/artifactory/r-npm-taobao-m-phantomjs"
taobao_operadriver_cdnurl="http://buildrepo.cs/artifactory/r-npm-taobao-m-operadriver/"
taobao_SQLITE3_BINARY_SITE="http://buildrepo.cs/artifactory/r-npm-taobao-m-sqlite3/"
taobao_profiler_binary_host_mirror="http://buildrepo.cs/artifactory/r-npm-taobao-m-nodeinspector/"

sourceDir=$(dirname $0)
echo ${sourceDir}

npmcmb() {
    # npm assets
    npm config set registry ${cmb_npm_registry}
    npm config set disturl ${cmb_disturl}
    # yarn
    yarn config set registry  ${cmb_npm_registry}
    echo "set cmb npm successful!"
}

npmtb() {
    npm config set registry ${taobao_npm_registry}
    npm config set disturl ${taobao_disturl}
    # yarn
    yarn config set registry  ${taobao_npm_registry}
    echo "set taobao npm successful!"
}

proxyext() {
    export HTTP_PROXY=${ext_proxy}
    export HTTPS_PROXY=${ext_proxy}
    export http_proxy=${ext_proxy}
    export https_proxy=${ext_proxy}
    echo "set http(s) proxy " ${ext_proxy}
}

proxycmb() {
    echo "[DEPRECATED] Now using custom ip route, please!"
    export HTTP_PROXY=${cmb_proxy}
    export HTTPS_PROXY=${cmb_proxy}
    export http_proxy=${cmb_proxy}
    export https_proxy=${cmb_proxy}
    export all_proxy=${cmb_proxy}
    echo "set http(s) proxy ${cmb_proxy}"
}

proxynull() {
    unset HTTP_PROXY
    unset http_proxy
    unset HTTPS_PROXY
    unset https_proxy
    echo "unset http(s) proxy"
}

gitcmb() {
    git config --global http.proxy ${cmb_proxy}
}

github() {
    git config --global http.proxy ${ext_proxy}
}

gitnull() {
    git config --global http.proxy null
}


case "$1" in
    npmcmb)
        npmcmb
        ;;
    npmtb)
        npmtb
        ;;
    proxyext)
        proxyext
        ;;
    proxycmb)
        proxycmb
        ;;
    proxynull)
        none
        ;;
    gitcmb)
        gitcmb
        ;;
    github)
        github
        ;;
    *)
        echo "Usage source ~/.cmbconfig.sh \n{npmcmb | npmtb | extnet | proxyext | proxycmb | proxynul | github | gitcmb } \n"
esac
