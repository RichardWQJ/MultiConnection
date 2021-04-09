一、mosquitto服务端进程启动：
    1、x86_64平台：
        ./third_party/lib/x86_64/mosquitto -c ./third_party/lib/x86_64/mosquitto.conf -v
    2、aarch64平台：
        ./third_party/lib/aarch64/mosquitto -c ./third_party/lib/aarch64/mosquitto.conf -v
    【注】启动参数：
        -c：指定配置文件；
        -v：打印详细信息；
        -d：后台运行

二、客户端编译运行：
    编译：
        make
    运行：
        1、PUB端运行：
            ./TEST pub
        2、SUB端运行
            ./TEST sub
        【注】如遇测试时动态库无法运行，可先运行以下命令：
        export LD_LIBRARY_PATH="./:$LD_LIBRARY_PATH"
        或者将库文件添加至系统运行库文件路径下.