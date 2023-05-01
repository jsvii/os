# ARoute原理

本文转载自 [https://juejin.cn/post/6844903648690962446](https://juejin.cn/post/6844903648690962446)

## 简介

本篇文章我会带着大家一步步手动实现路由框架来理解类似 ARouter 的路由框架原理，撸码的 demo 我会附在文末。本路由框架就叫 EaseRouter。\(注：demo 里搭建了组件化开发，组件化和路由本身并没有什么联系，但是两个单向依赖的组件之间需要互相启动对方的 Activity，因为没有相互引用，startActivity\(\) 是实现不了的，必须需要一个协定的通信的方式，此时类似 ARouter 和 ActivityRouter 的框架就派上用场了\)。

## 涉及知识点

* Router 框架原理

* apt、javapoet 知识

* Router 框架实现

### 第一节：组件化原理

本文的重点是对路由框架的实现进行介绍，所以对于组件化的基本知识在文中不会过多阐述，如有同学对组件化有不理解，可以参考网上众多的博客等介绍，然后再阅读 demo 中的组件化配置进行熟悉，这里附上 github demo 地址：[ARouter 原理剖析及手动实现，点我访问源码，欢迎 star](https://github.com/Xiasm/EasyRouter)。

![](https://user-gold-cdn.xitu.io/2018/7/30/164eaab5f9a7a9be?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)

如上图，在组件化中，为了业务逻辑的彻底解耦，同时也为了每个 module 都可以方便的单独运行和调试，上层的各个 module 不会进行相互依赖 \(只有在正式联调的时候才会让 app 壳 module 去依赖上层的其他组件 module\)，而是共同依赖于 base module，base module 中会依赖一些公共的第三方库和其他配置。那么在上层的各个 module 中，如何进行通信呢？

我们知道，传统的 Activity 之间通信，通过 startActivity\(intent\)，而在组件化的项目中，上层的 module 没有依赖关系 \(即便两个 module 有依赖关系，也只能是单向的依赖\)，那么假如 login module 中的一个 Activity 需要启动 pay\_module 中的一个 Activity 便不能通过 startActivity 来进行跳转。那么大家想一下还有什么其他办法呢？

可能有同学会想到隐式跳转，这当然也是一种解决方法，但是一个项目中不可能所有的跳转都是隐式的，这样 Manifest 文件会有很多过滤配置，而且非常不利于后期维护。当然你用反射也可以实现跳转，但是第一：大量的使用反射跳转对性能会有影响，第二：你需要拿到 Activity 的类文件，在组件开发的时候，想拿到其他 module 的类文件是很麻烦的（因为组件开发的时候组件 module 之间是没有相互引用的，你只能通过找到类的路径去拿到这个 class，显然非常麻烦），那么有没有一种更好的解决办法呢？办法当然是有的。下面看图：

![](https://user-gold-cdn.xitu.io/2018/7/30/164eaab5f9c56bb3?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)

在组件化中，我们通常都会在 base\_module 上层再依赖一个 router\_module, 而这个 router\_module 就是负责各个模块之间服务暴露和页面跳转的。

用过 ARouter 路由框架的同学应该都知道，在每个需要对其他 module 提供调用的 Activity 中，都会声明类似下面 @Route 注解，我们称之为路由地址

```java
@Route(path = "/main/main")
public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }
}


@Route(path = "/module1/module1main")
public class Module1MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_module1_main);
    }
}
```

那么这个注解有什么用呢，路由框架会在项目的编译器扫描所有添加 @Route 注解的 Activity 类，然后将 route 注解中的 path 地址和 Activity.class 文件一一对应保存，如直接保存在 map 中。为了让大家理解，我这里来使用近乎伪代码给大家简单演示一下。

```java
//项目编译后通过apt生成如下方法
public HashMap<String, ClassBean> routeInfo() {
    HashMap<String, ClassBean> route = new HashMap<String, ClassBean>();
    route.put("/main/main", MainActivity.class);
    route.put("/module1/module1main", Module1MainActivity.class);
    route.put("/login/login", LoginActivity.class);
}
```

这样我们想在 app 模块的 MainActivity 跳转到 login 模块的 LoginActivity，那么便只需调用如下：

```java
//不同模块之间启动Activity
public void login(String name, String password) {
    HashMap<String, ClassBean> route = routeInfo();
    LoginActivity.class classBean = route.get("/login/login");
    Intent intent = new Intent(this, classBean);
    intent.putExtra("name", name);
    intent.putExtra("password", password);
    startActivity(intent);
}
```

用过 ARouter 的同学应该知道，用 ARouter 启动 Activity 应该是下面这个写法

```java
// 2. Jump with parameters
ARouter.getInstance().build("/test/login")
            .withString("password", 666666)
            .withString("name", "小三")
            .navigation();
```

那么 ARouter 背后的原理是怎么样的呢？实际上它的核心思想跟上面讲解的是一样的，我们在代码里加入的 @Route 注解，会在编译时期通过 apt 生成一些存储 path 和 activityClass 映射关系的类文件，然后 app 进程启动的时候会拿到这些类文件，把保存这些映射关系的数据读到内存里 \(保存在 map 里\)，然后在进行路由跳转的时候，通过 build\(\) 方法传入要到达页面的路由地址，ARouter 会通过它自己存储的路由表找到路由地址对应的 Activity.class\(activity.class = map.get\(path\)\)，然后 new Intent\(\)，当调用 ARouter 的 withString\(\)方法它的内部会调用 intent.putExtra\(String name, String value\)，调用 navigation\(\)方法，它的内部会调用 startActivity\(intent\)进行跳转，这样便可以实现两个相互没有依赖的 module 顺利的启动对方的 Activity 了。

### 第二节：Route 注解的作用

简单讲，要通过 apt 生成我们的路由表，首先第一步需要定义注解

```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.CLASS)
public @interface Route {
    /**
     * 路由的路径
     * @return
     */
    String path();

    /**
     * 将路由节点进行分组，可以实现动态加载
     * @return
     */
    String group() default "";

}
```

这里看到 Route 注解里有 path 和 group，这便是仿照 ARouter 对路由进行分组。因为当项目变得越来越大庞大的时候，为了便于管理和减小首次加载路由表过于耗时的问题，我们对所有的路由进行分组。在 ARouter 中会要求路由地址至少需要两级，如 "/xx/xx", 一个模块下可以有多个分组。这里我们就将路由地址定为必须大于等于两级，其中第一级是 group。如 app module 下的路由注解：

```java
@Route(path = "/main/main")
public class MainActivity extends AppCompatActivity {}

@Route(path = "/main/main2")
public class Main2Activity extends AppCompatActivity {}

@Route(path = "/show/info")
public class ShowActivity extends AppCompatActivity {}
```

在项目编译的时候，我们将会通过 apt 生成 EaseRouter\_Root\_app 文件和 EaseRouter\_Group\_main、EEaseRouter\_Group\_show 等文件，EaseRouter\_Root\_app 文件对应于 app module，里面记录着本 module 下所有的分组信息，EaseRouter\_Group\_main、EaseRouter\_Group\_show 文件分别记载着当前分组的所有路由地址和 ActivityClass 映射信息。  
本 demo 在编译的时候会生成类如下所示，先不要管这些类是怎么生成的，仔细看类的内容

```java
public class EaseRouter_Root_app implements IRouteRoot {
  @Override
  public void loadInto(Map<String, Class<? extends IRouteGroup>> routes) {
    routes.put("main", EaseRouter_Group_main.class);
    routes.put("show", EaseRouter_Group_show.class);
  }
}


public class EaseRouter_Group_main implements IRouteGroup {
  @Override
  public void loadInto(Map<String, RouteMeta> atlas) {
    atlas.put("/main/main",RouteMeta.build(RouteMeta.Type.ACTIVITY,Main2\Activity.class,"/main/main","main"));
    atlas.put("/main/main2",RouteMeta.build(RouteMeta.Type.ACTIVITY,Main2\Activity.class,"/main/main2","main"));
  }
}

public class EaseRouter_Group_show implements IRouteGroup {
  @Override
  public void loadInto(Map<String, RouteMeta> atlas) {
    atlas.put("/show/info",RouteMeta.build(RouteMeta.Type.ACTIVITY,ShowActivity.class,"/show/info","show"));
  }
}
```

大家会看到生成的类分别实现了 IRouteRoot 和 IRouteGroup 接口，并且实现了 loadInto\(\) 方法，而 loadInto 方法通过传入一个特定类型的 map 就能把分组信息放入 map 里。这两个接口是干嘛的我们先搁置，继续往下看  
如果我们在 login\_module 中想启动 app\_module 中的 MainActivity 类，首先，我们已知 MainActivity 类的路由地址是 "/main/main"，第一个 "/main" 代表分组名，那么我们岂不是可以像下面这样调用去得到 MainActivity 类文件，然后 startActivity。这里的 RouteMeta 只是存有 Activity class 文件的封装类，先不用理会。

```java
public void test() {
    EaseRouter_Root_app rootApp = new EaseRouter_Root_app();
    HashMap<String, Class<? extends IRouteGroup>> rootMap = new HashMap<>();
    rootApp.loadInto(rootMap);

    //得到/main分组
    Class<? extends IRouteGroup> aClass = rootMap.get("main");
    try {
        HashMap<String, RouteMeta> groupMap = new HashMap<>();
        aClass.newInstance().loadInto(groupMap);
        //得到MainActivity
        RouteMeta main = groupMap.get("/main/main");
        Class<?> mainActivityClass = main.getDestination();

        Intent intent = new Intent(this, mainActivityClass);
        startActivity(intent);
    } catch (InstantiationException e) {
        e.printStackTrace();
    } catch (IllegalAccessException e) {
        e.printStackTrace();
    }

}
```

可以看到，只要有了这些实现了 IRouteRoot 和 IRouteGroup 的类文件，我们便能轻易的启动其他 module 的 Activity 了。这些类文件，我们可以约定好之后，在代码的编写过程中自己动手实现，也可以通过 apt 生成。作为一个框架，当然是自动解析 Route 注解然后生成这些类文件更好了。那么就看下节，如何去生成这些文件。

### 第三节：apt 和 javapoet 详解

```text
通过上节我们知道在 Activity 类上加上 @Route 注解之后，便可通过 apt 来生成对应的路由表，那么这节我们就来讲述一下如何通过 apt 来生成路由表。这节我会拿着 demo 里面的代码来跟大家详细介绍，我们先来了解一下 apt 吧！  
APT 是 Annotation Processing Tool 的简称, 即注解处理工具。它是在编译期对代码中指定的注解进行解析，然后做一些其他处理（如通过 javapoet 生成新的 Java 文件）。我们常用的 ButterKnife，其原理就是通过注解处理器在编译期扫描代码中加入的 @BindView、@OnClick 等注解进行扫描处理，然后生成 XXX_ViewBinding 类，实现了 view 的绑定。  

第一步：定义注解处理器，用来在编译期扫描加入 @Route 注解的类，然后做处理。  
```

这也是 apt 最核心的一步，新建 RouterProcessor 继承自 AbstractProcessor, 然后实现 process 方法。在项目编译期会执行 RouterProcessor 的 process\(\) 方法，我们便可以在这个方法里处理 Route 注解了。此时我们需要为 RouterProcessor 指明它需要处理什么注解，这里引入一个 google 开源的自动注册工具 AutoService，如下依赖 \(也可以手动进行注册，不过略微麻烦\)：

```java
implementation 'com.google.auto.service:auto-service:1.0-rc2'
```

这个工具可以通过添加注解来为 RouterProcessor 指定它需要的配置 \(当然也可以自己手动去配置，不过会有点麻烦\)，如下所示

```java
@AutoService(Processor.class)
public class RouterProcessor extends AbstractProcessor {

  //...
}
```

完整的 RouterProcessor 注解处理器配置如下：

```java
@AutoService(Processor.class)
/**
  处理器接收的参数 替代 {@link AbstractProcessor
 */
@SupportedOptions(Constant.ARGUMENTS_NAME)
/**
 * 指定使用的Java版本 替代 {@link AbstractProcessor
 */
@SupportedSourceVersion(SourceVersion.RELEASE_7)
/**
 * 注册给哪些注解的  替代 {@link AbstractProcessor
 */
@SupportedAnnotationTypes(Constant.ANNOTATION_TYPE_ROUTE)

public class RouterProcessor extends AbstractProcessor {
    /**
     * key:组名 value:类名
     */
    private Map<String, String> rootMap = new TreeMap<>();
    /**
     * 分组 key:组名 value:对应组的路由信息
     */
    private Map<String, List<RouteMeta>> groupMap = new HashMap<>();

    /**
     * 节点工具类 (类、函数、属性都是节点)
     */
    private Elements elementUtils;

    /**
     * type(类信息)工具类
     */
    private Types typeUtils;

    /**
     * 文件生成器 类/资源
     */
    private Filer filerUtils;

    private String moduleName;

    private Log log;

    @Override
    public synchronized void init(ProcessingEnvironment processingEnvironment) {
        super.init(processingEnvironment);
        //获得apt的日志输出
        log = Log.newLog(processingEnvironment.getMessager());
        elementUtils = processingEnvironment.getElementUtils();
        typeUtils = processingEnvironment.getTypeUtils();
        filerUtils = processingEnvironment.getFiler();

        //参数是模块名 为了防止多模块/组件化开发的时候 生成相同的 xx$$ROOT?文件
        Map<String, String> options = processingEnvironment.getOptions();
        if (!Utils.isEmpty(options)) {
            moduleName = options.get(Constant.ARGUMENTS_NAME);
        }
        if (Utils.isEmpty(moduleName)) {
            throw new RuntimeException("Not set processor moudleName option !");
        }
        log.i("init RouterProcessor " + moduleName + " success !");
    }

    /**
     *
     * @param set 使用了支持处理注解的节点集合
     * @param roundEnvironment 表示当前或是之前的运行环境,可以通过该对象查找找到的注解。
     * @return true 表示后续处理器不会再处理(已经处理)
     */
    @Override
    public boolean process(Set<? extends TypeElement> set, RoundEnvironment roundEnvironment) {
        if (!Utils.isEmpty(set)) {
            //被Route注解的节点集合
            Set<? extends Element> rootElements = roundEnvironment.getElementsAnnotatedWith(Route.class);
            if (!Utils.isEmpty(rootElements)) {
                processorRoute(rootElements);
            }
            return true;
        }
        return false;
    }


    //...

}
```

我们通过 @SupportedOptions\(Constant.ARGUMENTS\_NAME\) 拿到每个 module 的名字，用来生成对应 module 下存放路由信息的类文件名。在这之前，我们需要在 module 的 gradle 下配置如下

```java
javaCompileOptions {
            annotationProcessorOptions {
                arguments = [moduleName: project.getName()]
            }
        }
```

Constant.ARGUMENTS\_NAME 便是每个 module 的名字。

@SupportedAnnotationTypes\(Constant.ANNOTATION\_TYPE\_ROUTE\) 指定了需要处理的注解的路径地址, 在此就是 Route.class 的路径地址。

RouterProcessor 中我们实现了 init 方法，拿到 log apt 日志输出工具用以输出 apt 日志信息, 并通过以下代码得到上面提到的每个 module 配置的 moduleName

```java
//参数是模块名 为了防止多模块/组件化开发的时候 生成相同的 xx$$ROOT?文件
Map<String, String> options = processingEnvironment.getOptions();
if (!Utils.isEmpty(options)) {
    moduleName = options.get(Constant.ARGUMENTS_NAME);
}
if (Utils.isEmpty(moduleName)) {
    throw new RuntimeException("Not set processor moudleName option !");
}
```

```text
 第二步，在 process() 方法里开始生成 EaseRouter_Route_moduleName 类文件和 EaseRouter_Group_moduleName 文件。这里在 process()里生成文件用 javapoet，这是 squareup 公司开源的一个库，通过调用它的 api，可以很方便的生成 java 文件，在含有注解处理器 (demo 中 apt 相关的代码实现都在 easy-compiler module 中) 的 module 中引入依赖如下：
```

```java
implementation 'com.squareup:javapoet:1.7.0'
```

好了，我们终于可以生成文件了, 在 process\(\) 方法里有如下代码，

```java
if (!Utils.isEmpty(set)) {
    //被Route注解的节点集合
    Set<? extends Element> rootElements = roundEnvironment.getElementsAnnotatedWith(Route.class);
    if (!Utils.isEmpty(rootElements)) {
        processorRoute(rootElements);
    }
    return true;
}
return false;
```

set 就是扫描得到的支持处理注解的节点集合，然后得到 rootElements，即被 @Route 注解的节点集合，此时就可以调用 processorRoute\(rootElements\) 方法去生成文件了。processorRoute\(rootElements\) 方法实现如下：

```java
private void processorRoute(Set<? extends Element> rootElements) {
    //获得Activity这个类的节点信息
    TypeElement activity = elementUtils.getTypeElement(Constant.ACTIVITY);
    TypeElement service = elementUtils.getTypeElement(Constant.ISERVICE);
    for (Element element : rootElements) {
        RouteMeta routeMeta;
        //类信息
        TypeMirror typeMirror = element.asType();
        log.i("Route class:" + typeMirror.toString());
        Route route = element.getAnnotation(Route.class);
        if (typeUtils.isSubtype(typeMirror, activity.asType())) {
            routeMeta = new RouteMeta(RouteMeta.Type.ACTIVITY, route, element);
        } else if (typeUtils.isSubtype(typeMirror, service.asType())) {
            routeMeta = new RouteMeta(RouteMeta.Type.ISERVICE, route, element);
        } else {
            throw new RuntimeException("Just support Activity or IService Route: " + element);
        }
        categories(routeMeta);
    }
    TypeElement iRouteGroup = elementUtils.getTypeElement(Constant.IROUTE_GROUP);
    TypeElement iRouteRoot = elementUtils.getTypeElement(Constant.IROUTE_ROOT);

    //生成Group记录分组表
    generatedGroup(iRouteGroup);

    //生成Root类 作用：记录<分组，对应的Group类>
    generatedRoot(iRouteRoot, iRouteGroup);
}
```

上节中提到过生成的 root 文件和 group 文件分别实现了 IRouteRoot 和 IRouteGroup 接口，就是通过下面这两行文件代码拿到 IRootGroup 和 IRootRoot 的字节码信息，然后传入 generatedGroup\(iRouteGroup\) 和 generatedRoot\(iRouteRoot, iRouteGroup\) 方法，这两个方法内部会通过 javapoet api 生成 java 文件，并实现这两个接口。

```java
TypeElement iRouteGroup = elementUtils.getTypeElement(Constant.IROUTE_GROUP);
TypeElement iRouteRoot = elementUtils.getTypeElement(Constant.IROUTE_ROOT);
```

generatedGroup\(iRouteGroup\) 和 generatedRoot\(iRouteRoot, iRouteGroup\) 就是生成上面提到的 EaseRouter\_Root\_app 和 EaseRouter\_Group\_main 等文件的具体实现，代码太多，我粘出一个实现供大家参考，其实生成 java 文件的思路都是一样的，我们只需要熟悉 javapoet 的 api 如何使用即可。大家可以后续在 demo 里详细分析，这里我只是讲解核心的实现。

```java
/**
 * 生成Root类  作用：记录<分组，对应的Group类>
 * @param iRouteRoot
 * @param iRouteGroup
 */
private void generatedRoot(TypeElement iRouteRoot, TypeElement iRouteGroup) {
    //创建参数类型 Map<String,Class<? extends IRouteGroup>> routes>
    //Wildcard 通配符
    ParameterizedTypeName parameterizedTypeName = ParameterizedTypeName.get(
            ClassName.get(Map.class),
            ClassName.get(String.class),
            ParameterizedTypeName.get(
                    ClassName.get(Class.class),
                    WildcardTypeName.subtypeOf(ClassName.get(iRouteGroup))
            ));
    //参数 Map<String,Class<? extends IRouteGroup>> routes> routes
    ParameterSpec parameter = ParameterSpec.builder(parameterizedTypeName, "routes").build();

    //函数 public void loadInfo(Map<String,Class<? extends IRouteGroup>> routes> routes)
    MethodSpec.Builder methodBuilder = MethodSpec.methodBuilder(Constant.METHOD_LOAD_INTO)
            .addModifiers(Modifier.PUBLIC)
            .addAnnotation(Override.class)
            .addParameter(parameter);
    //函数体
    for (Map.Entry<String, String> entry : rootMap.entrySet()) {
        methodBuilder.addStatement("routes.put($S, $T.class)", entry.getKey(), ClassName.get(Constant.PACKAGE_OF_GENERATE_FILE, entry.getValue()));
    }
    //生成$Root$类
    String className = Constant.NAME_OF_ROOT + moduleName;
    TypeSpec typeSpec = TypeSpec.classBuilder(className)
            .addSuperinterface(ClassName.get(iRouteRoot))
            .addModifiers(Modifier.PUBLIC)
            .addMethod(methodBuilder.build())
            .build();
    try {
        JavaFile.builder(Constant.PACKAGE_OF_GENERATE_FILE, typeSpec).build().writeTo(filerUtils);
        log.i("Generated RouteRoot：" + Constant.PACKAGE_OF_GENERATE_FILE + "." + className);
    } catch (IOException e) {
        e.printStackTrace();
    }
}
```

可以看到，ParameterizedTypeName 是创建参数类型的 api，ParameterSpec 是创建参数的实现，MethodSpec 是函数的生成实现等等。最后，当参数、方法、类信息都准备好了之后，调用 JavaFileapi 生成类文件。JavaFile 的 builder \(\) 方法传入了 PACKAGE\_OF\_GENERATE\_FILE 变量，这个就是指定生成的类文件的目录，方便我们在 app 进程启动的时候去遍历拿到这些类文件。

### 第四节 实现路由框架的初始化

```text
通过前几节的讲解，我们知道了看似很复杂的路由框架，其实原理很简单，我们可以理解为一个 map(其实是两个 map，一个保存 group 列表，一个保存 group 下的路由地址和 activityClass 关系) 保存了路由地址和 ActivityClass 的映射关系，然后通过 map.get("router address") 拿到 AncivityClass，通过 startActivity() 调用就好了。但一个框架的设计要考虑的事情远远没有这么简单。下面我们就来分析一下：  

要实现这么一个路由框架，首先我们需要在用户使用路由跳转之前把这些路由映射关系拿到手，拿到这些路由关系最好的时机就是应用程序初始化的时候，前面的讲解中我贴过几行代码，是通过 apt 生成的路由映射关系文件，为了方便大家理解，我把这些文件重新粘贴到下面代码中（这几个类都是单独的文件，在项目编译后会在各个模块的 / build/generated/source/apt 文件夹下面生成，为了演示方便我只贴出来了 app 模块下生成的类，其他模块如 module1、module2 下面的类跟 app 下面的没有什么区别），在程序启动的时候扫描这些生成的类文件，然后获取到映射关系信息，保存起来。
```

```java
public class EaseRouter_Root_app implements IRouteRoot {
  @Override
  public void loadInto(Map<String, Class<? extends IRouteGroup>> routes) {
    routes.put("main", EaseRouter_Group_main.class);
    routes.put("show", EaseRouter_Group_show.class);
  }
}


public class EaseRouter_Group_main implements IRouteGroup {
  @Override
  public void loadInto(Map<String, RouteMeta> atlas) {
    atlas.put("/main/main",RouteMeta.build(RouteMeta.Type.ACTIVITY,Main2\Activity.class,"/main/main","main"));
    atlas.put("/main/main2",RouteMeta.build(RouteMeta.Type.ACTIVITY,Main2\Activity.class,"/main/main2","main"));
  }
}

public class EaseRouter_Group_show implements IRouteGroup {
  @Override
  public void loadInto(Map<String, RouteMeta> atlas) {
    atlas.put("/show/info",RouteMeta.build(RouteMeta.Type.ACTIVITY,ShowActivity.class,"/show/info","show"));
  }
}
```

```text
可以看到，这些文件中，实现了 IRouteRoot 接口的类都是保存了 group 分组映射信息，实现了 IRouteGroup 接口的类都保存了单个分组下的路由映射信息。只要我们得到实现 IRouteRoot 接口的所有类文件，便能通过循环调用它的 loadInfo()方法得到所有实现 IRouteGroup 接口的类，而所有实现 IRouteGroup 接口的类里面保存了项目的所有路由信息。IRouteGroup 的 loadInfo()方法，通过传入一个 map，便会将这个分组里的映射信息存入 map 里。可以看到 map 里的 value 是 “RouteMeta.build(RouteMeta.Type.ACTIVITY,ShowActivity.class,"/show/info","show")”,RouteMeta.build() 会返回 RouteMeta，RouteMeta 里面便保存着 ActivityClass 的所有信息。那么我们这个框架，就有了第一个功能需求，便是在 app 进程启动的时候进行框架的初始化(或者在你开始用路由跳转之前进行初始化都可以)，在初始化中拿到映射关系信息，保存在 map 里，以便程序运行中可以快速找到路由映射信息实现跳转。下面看具体的初始化代码。  
```

注：这里我们只讲解大体的思路，不会细致到讲解每一个方法每一行代码的具体作用，跟着我的思路你会明白框架设计的具体细节，每一步要实现的功能是什么，但是精确到方法和每一行代码的具体含义你还需要仔细研读 demo。

```java
public class MyApplication extends Application {

    @Override
    public void onCreate() {
        super.onCreate();
        EasyRouter.init(this);
    }
}

public class EasyRouter {

  private static final String TAG = "EasyRouter";
   private static final String ROUTE_ROOT_PAKCAGE = "com.xsm.easyrouter.routes";
   private static final String SDK_NAME = "EaseRouter";
   private static final String SEPARATOR = "_";
   private static final String SUFFIX_ROOT = "Root";

   private static EasyRouter sInstance;
   private static Application mContext;
   private Handler mHandler;

   private EasyRouter() {
       mHandler = new Handler(Looper.getMainLooper());
   }

   public static EasyRouter getsInstance() {
       synchronized (EasyRouter.class) {
           if (sInstance == null) {
               sInstance = new EasyRouter();
           }
       }
       return sInstance;
   }

   public static void init(Application application) {
       mContext = application;
       try {
           loadInfo();
       } catch (Exception e) {
           e.printStackTrace();
           Log.e(TAG, "初始化失败!", e);
       }
   }

   //...
}
```

可以看到，init\(\) 方法中调用了 loadInfo\(\) 方法，而这个 loadInfo\(\) 便是我们初始化的核心。

```java
private static void loadInfo() throws PackageManager.NameNotFoundException, InterruptedException, ClassNotFoundException, NoSuchMethodException, IllegalAccessException, InvocationTargetException, InstantiationException {
    //获得所有 apt生成的路由类的全类名 (路由表)
    Set<String> routerMap = ClassUtils.getFileNameByPackageName(mContext, ROUTE_ROOT_PAKCAGE);
    for (String className : routerMap) {
        if (className.startsWith(ROUTE_ROOT_PAKCAGE + "." + SDK_NAME + SEPARATOR + SUFFIX_ROOT)) {
            //root中注册的是分组信息 将分组信息加入仓库中
            ((IRouteRoot) Class.forName(className).getConstructor().newInstance()).loadInto(Warehouse.groupsIndex);
        }
    }
    for (Map.Entry<String, Class<? extends IRouteGroup>> stringClassEntry : Warehouse.groupsIndex.entrySet()) {
        Log.d(TAG, "Root映射表[ " + stringClassEntry.getKey() + " : " + stringClassEntry.getValue() + "]");
    }

}
```

我们首先通过 ClassUtils.getFileNameByPackageName\(mContext, ROUTE\_ROOT\_PAKCAGE\) 得到 apt 生成的所有实现 IRouteRoot 接口的类文件集合，通过上面的讲解我们知道，拿到这些类文件便可以得到所有的 routerAddress---activityClass 映射关系。  
这个 ClassUtils.getFileNameByPackageName\(\) 方法就是具体的实现了，下面我们看具体的代码：

```java
   /**
     * 得到路由表的类名
     * @param context
     * @param packageName
     * @return
     * @throws PackageManager.NameNotFoundException
     * @throws InterruptedException
     */
    public static Set<String> getFileNameByPackageName(Application context, final String packageName)
            throws PackageManager.NameNotFoundException, InterruptedException {
        final Set<String> classNames = new HashSet<>();
        List<String> paths = getSourcePaths(context);
        //使用同步计数器判断均处理完成
        final CountDownLatch countDownLatch = new CountDownLatch(paths.size());
        ThreadPoolExecutor threadPoolExecutor = DefaultPoolExecutor.newDefaultPoolExecutor(paths.size());
        for (final String path : paths) {
            threadPoolExecutor.execute(new Runnable() {
                @Override
                public void run() {
                    DexFile dexFile = null;
                    try {
                        //加载 apk中的dex 并遍历 获得所有包名为 {packageName} 的类
                        dexFile = new DexFile(path);
                        Enumeration<String> dexEntries = dexFile.entries();
                        while (dexEntries.hasMoreElements()) {
                            String className = dexEntries.nextElement();
                            if (!TextUtils.isEmpty(className) && className.startsWith(packageName)) {
                                classNames.add(className);
                            }
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    } finally {
                        if (null != dexFile) {
                            try {
                                dexFile.close();
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                        }
                        //释放一个
                        countDownLatch.countDown();
                    }
                }
            });
        }
        //等待执行完成
        countDownLatch.await();
        return classNames;
    }
```

这个方法会通过开启子线程，去扫描 apk 中所有的 dex，遍历找到所有包名为 packageName 的类名，然后将类名再保存到 classNames 集合里。  
List paths = getSourcePaths\(context\) 这句代码会获得所有的 apk 文件 \(instant run 会产生很多 split apk\), 这个方法的具体实现大家 kandemo 即可，不再阐述。这里用到了 CountDownLatch 类，会分 path 一个文件一个文件的检索，等到所有的类文件都找到后便会返回这个 Set 集合。所以我们可以知道，初始化时找到这些类文件会有一定的耗时，所以 ARouter 这里会有一些优化，只会遍历找一次类文件，找到之后就会保存起来，下次 app 进程启动会检索是否有保存这些文件，如果有就会直接调用保存后的数据去初始化。

### 第五节 路由跳转

```text
通过上节的介绍，我们知道在初始化的时候已经拿到了所有的路由信息，那么实现跳转便好做了。
```

```java
@Route(path = "/main/main")
public class MainActivity extends AppCompatActivity {

  public void startModule1MainActivity(View view) {
    EasyRouter.getsInstance().build("/module1/module1main").navigation();
  }

}
```

在 build 的时候，传入要跳转的路由地址，build\(\) 方法会返回一个 Postcard 对象，我们称之为跳卡。然后调用 Postcard 的 navigation\(\) 方法完成跳转。用过 ARouter 的对这个跳卡都应该很熟悉吧！Postcard 里面保存着跳转的信息。下面我把 Postcard 类的代码实现粘下来：

```java
public class Postcard extends RouteMeta {
    private Bundle mBundle;
    private int flags = -1;
    //新版风格
    private Bundle optionsCompat;
    //老版
    private int enterAnim;
    private int exitAnim;

    //服务
    private IService service;

    public Postcard(String path, String group) {
        this(path, group, null);
    }

    public Postcard(String path, String group, Bundle bundle) {
        setPath(path);
        setGroup(group);
        this.mBundle = (null == bundle ? new Bundle() : bundle);
    }

    public Bundle getExtras() {return mBundle;}

    public int getEnterAnim() {return enterAnim;}

    public int getExitAnim() {return exitAnim;}

    public IService getService() {
        return service;
    }

    public void setService(IService service) {
        this.service = service;
    }

    /**
     * Intent.FLAG_ACTIVITY**
     * @param flag
     * @return
     */
    public Postcard withFlags(int flag) {
        this.flags = flag;
        return this;
    }

    public int getFlags() {
        return flags;
    }

    /**
     * 跳转动画
     *
     * @param enterAnim
     * @param exitAnim
     * @return
     */
    public Postcard withTransition(int enterAnim, int exitAnim) {
        this.enterAnim = enterAnim;
        this.exitAnim = exitAnim;
        return this;
    }

    /**
     * 转场动画
     *
     * @param compat
     * @return
     */
    public Postcard withOptionsCompat(ActivityOptionsCompat compat) {
        if (null != compat) {
            this.optionsCompat = compat.toBundle();
        }
        return this;
    }

    public Postcard withString(@Nullable String key, @Nullable String value) {
        mBundle.putString(key, value);
        return this;
    }


    public Postcard withBoolean(@Nullable String key, boolean value) {
        mBundle.putBoolean(key, value);
        return this;
    }


    public Postcard withShort(@Nullable String key, short value) {
        mBundle.putShort(key, value);
        return this;
    }


    public Postcard withInt(@Nullable String key, int value) {
        mBundle.putInt(key, value);
        return this;
    }


    public Postcard withLong(@Nullable String key, long value) {
        mBundle.putLong(key, value);
        return this;
    }


    public Postcard withDouble(@Nullable String key, double value) {
        mBundle.putDouble(key, value);
        return this;
    }


    public Postcard withByte(@Nullable String key, byte value) {
        mBundle.putByte(key, value);
        return this;
    }


    public Postcard withChar(@Nullable String key, char value) {
        mBundle.putChar(key, value);
        return this;
    }


    public Postcard withFloat(@Nullable String key, float value) {
        mBundle.putFloat(key, value);
        return this;
    }


    public Postcard withParcelable(@Nullable String key, @Nullable Parcelable value) {
        mBundle.putParcelable(key, value);
        return this;
    }


    public Postcard withStringArray(@Nullable String key, @Nullable String[] value) {
        mBundle.putStringArray(key, value);
        return this;
    }


    public Postcard withBooleanArray(@Nullable String key, boolean[] value) {
        mBundle.putBooleanArray(key, value);
        return this;
    }


    public Postcard withShortArray(@Nullable String key, short[] value) {
        mBundle.putShortArray(key, value);
        return this;
    }


    public Postcard withIntArray(@Nullable String key, int[] value) {
        mBundle.putIntArray(key, value);
        return this;
    }


    public Postcard withLongArray(@Nullable String key, long[] value) {
        mBundle.putLongArray(key, value);
        return this;
    }


    public Postcard withDoubleArray(@Nullable String key, double[] value) {
        mBundle.putDoubleArray(key, value);
        return this;
    }


    public Postcard withByteArray(@Nullable String key, byte[] value) {
        mBundle.putByteArray(key, value);
        return this;
    }


    public Postcard withCharArray(@Nullable String key, char[] value) {
        mBundle.putCharArray(key, value);
        return this;
    }


    public Postcard withFloatArray(@Nullable String key, float[] value) {
        mBundle.putFloatArray(key, value);
        return this;
    }


    public Postcard withParcelableArray(@Nullable String key, @Nullable Parcelable[] value) {
        mBundle.putParcelableArray(key, value);
        return this;
    }

    public Postcard withParcelableArrayList(@Nullable String key, @Nullable ArrayList<? extends
            Parcelable> value) {
        mBundle.putParcelableArrayList(key, value);
        return this;
    }

    public Postcard withIntegerArrayList(@Nullable String key, @Nullable ArrayList<Integer> value) {
        mBundle.putIntegerArrayList(key, value);
        return this;
    }

    public Postcard withStringArrayList(@Nullable String key, @Nullable ArrayList<String> value) {
        mBundle.putStringArrayList(key, value);
        return this;
    }

    public Bundle getOptionsBundle() {
        return optionsCompat;
    }

    public Object navigation() {
        return EasyRouter.getsInstance().navigation(null, this, -1, null);
    }

    public Object navigation(Context context) {
        return EasyRouter.getsInstance().navigation(context, this, -1, null);
    }


    public Object navigation(Context context, NavigationCallback callback) {
        return EasyRouter.getsInstance().navigation(context, this, -1, callback);
    }

    public Object navigation(Context context, int requestCode) {
        return EasyRouter.getsInstance().navigation(context, this, requestCode, null);
    }

    public Object navigation(Context context, int requestCode, NavigationCallback callback) {
        return EasyRouter.getsInstance().navigation(context, this, requestCode, callback);
    }


}
```

如果你是一个 Android 开发，Postcard 类里面的东西就不用我再给你介绍了吧！（哈哈）我相信你一看就明白了。我们只介绍一个方法 navigation\(\)，他有好几个重载方法，方法里面会调用 EasyRouter 类的 navigation\(\) 方法。EaseRouter 的 navigation\(\) 方法，就是跳转的核心了。下面请看：

```java
protected Object navigation(Context context, final Postcard postcard, final int requestCode, final NavigationCallback callback) {
    try {
        prepareCard(postcard);
    }catch (NoRouteFoundException e) {
        e.printStackTrace();
        //没找到
        if (null != callback) {
            callback.onLost(postcard);
        }
        return null;
    }
    if (null != callback) {
        callback.onFound(postcard);
    }

    switch (postcard.getType()) {
        case ACTIVITY:
            final Context currentContext = null == context ? mContext : context;
            final Intent intent = new Intent(currentContext, postcard.getDestination());
            intent.putExtras(postcard.getExtras());
            int flags = postcard.getFlags();
            if (-1 != flags) {
                intent.setFlags(flags);
            } else if (!(currentContext instanceof Activity)) {
                intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            }
            mHandler.post(new Runnable() {
                @Override
                public void run() {
                    //可能需要返回码
                    if (requestCode > 0) {
                        ActivityCompat.startActivityForResult((Activity) currentContext, intent,
                                requestCode, postcard.getOptionsBundle());
                    } else {
                        ActivityCompat.startActivity(currentContext, intent, postcard
                                .getOptionsBundle());
                    }

                    if ((0 != postcard.getEnterAnim() || 0 != postcard.getExitAnim()) &&
                            currentContext instanceof Activity) {
                        //老版本
                        ((Activity) currentContext).overridePendingTransition(postcard
                                        .getEnterAnim()
                                , postcard.getExitAnim());
                    }
                    //跳转完成
                    if (null != callback) {
                        callback.onArrival(postcard);
                    }
                }
            });
            break;
        case ISERVICE:
            return postcard.getService();
        default:
            break;
    }
    return null;
}
```

这个方法里先去调用了 prepareCard\(postcard\) 方法，prepareCard\(postcard\) 代码我贴出来，

```java
private void prepareCard(Postcard card) {
    RouteMeta routeMeta = Warehouse.routes.get(card.getPath());
    if (null == routeMeta) {
        Class<? extends IRouteGroup> groupMeta = Warehouse.groupsIndex.get(card.getGroup());
        if (null == groupMeta) {
            throw new NoRouteFoundException("没找到对应路由：分组=" + card.getGroup() + "   路径=" + card.getPath());
        }
        IRouteGroup iGroupInstance;
        try {
            iGroupInstance = groupMeta.getConstructor().newInstance();
        } catch (Exception e) {
            throw new RuntimeException("路由分组映射表记录失败.", e);
        }
        iGroupInstance.loadInto(Warehouse.routes);
        //已经准备过了就可以移除了 (不会一直存在内存中)
        Warehouse.groupsIndex.remove(card.getGroup());
        //再次进入 else
        prepareCard(card);
    } else {
        //类 要跳转的activity 或IService实现类
        card.setDestination(routeMeta.getDestination());
        card.setType(routeMeta.getType());
        switch (routeMeta.getType()) {
            case ISERVICE:
                Class<?> destination = routeMeta.getDestination();
                IService service = Warehouse.services.get(destination);
                if (null == service) {
                    try {
                        service = (IService) destination.getConstructor().newInstance();
                        Warehouse.services.put(destination, service);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
                card.setService(service);
                break;
            default:
                break;
        }
    }
}
```

注意，Warehouse 就是专门用来存放路由映射关系的类，这在 ARouter 里面也是。这段代码 Warehouse.routes.get\(card.getPath\(\)\)通过 path 拿到对应的 RouteMeta，这个 RouteMeta 里面保存了 activityClass 等信息。继续往下看，如果判断拿到的 RouteMeta 是空，说明这个路由地址还没有加载到 map 里面 \(为了效率，这里是用了懒加载\)，只有在第一次用到当前路由地址的时候，会去 Warehouse.routes 里面拿 routeMeta，如果拿到的是空，会根据当前路由地址的 group 拿到对应的分组，通过反射创建实例，然后调用实例的 loadInfo 方法，把它里面保存的映射信息添加到 Warehouse.routes 里面，并且再次调用 prepareCard\(card\)，这时再通过 Warehouse.routes.get\(card.getPath\(\)\) 就可以顺利拿到 RouteMeta 了。进入 else{}里面，调用了 card.setDestination\(routeMeta.getDestination\(\)\)，这个 setDestination 就是将 RouteMeta 里面保存的 activityClass 放入 Postcard 里面，下面 switch 代码块可以先不用看，这是实现 ARouter 中通过依赖注入实现 Provider 服务的逻辑，有心研究的同学可以去读一下 demo。  
好了，prepareCard\(\) 方法调用完成后，我们的 postcard 里面就保存了 activityClass，然后 switch \(postcard.getType\(\)\){} 会判断 postcard 的 type 为 ACTIVITY，然后通过 ActivityCompat.startActivity 启动 Activity。到这里，路由跳转的实现已经讲解完毕了。

### 小结

EaseRouter 本身只是参照 ARouter 手动实现的路由框架，并且剔除掉了很多东西，如过滤器等，如果想要用在项目里，建议还是用 ARouter 更好 \(毕竟这只是个练手项目，功能也不够全面，当然有同学想对 demo 扩展后使用那当然更好，遇到什么问题及时联系我\)。我的目的是通过自己手动实现来加深对知识的理解，这里面涉及到的知识点如 apt、javapoet 和组件化思路、编写框架的思路等。看到这里，如果感觉干货很多，欢迎点个 star 或分享给更多人。

### demo 地址

[仿 ARouter 一步步实现一个路由框架，点我访问源码，欢迎 star](https://github.com/Xiasm/EasyRouter)

### 联系方式

[email:xiasem@163.com](mailto:email:xiasem@163.com) 

