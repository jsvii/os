* androidx

https://blog.csdn.net/guolin_blog/article/details/97142065

Android系统在刚刚面世的时候，可能连它的设计者也没有想到它会如此成功，因此也不可能在一开始的时候就将它的API考虑的非常周全。随着Android系统版本不断地迭代更新，每个版本中都会加入很多新的API进去，但是新增的API在老版系统中并不存在，因此这就出现了一个向下兼容的问题。

举个例子，当Android系统发布到3.0版本的时候，突然意识到了平板电脑的重要性，因此为了让Android可以更好地兼容平板，Android团队在3.0系统（API 11）中加入了Fragment功能。但是Fragment的作用并不只局限于平板，以前的老系统中也想使用这个功能该怎么办？于是Android团队推出了一个鼎鼎大名的Android Support Library，用于提供向下兼容的功能。比如我们每个人都熟知的support-v4库，appcompat-v7库都是属于Android Support Library的，这两个库相信任何做过Android开发的人都使用过。

但是可能很多人并没有考虑过support-v4库的名字到底是什么意思，这里跟大家解释一下。4在这里指的是Android API版本号，对应的系统版本是1.6。那么support-v4的意思就是这个库中提供的API会向下兼容到Android 1.6系统。它对应的包名如下：

类似地，appcompat-v7指的是将库中提供的API向下兼容至API 7，也就是Android 2.1系统。它对应的包名如下：

可以发现，Android Support Library中提供的库，它们的包名都是以android.support.*开头的。

但是慢慢随着时间的推移，什么1.6、2.1系统早就已经被淘汰了，现在Android官方支持的最低系统版本已经是4.0.1，对应的API版本号是15。support-v4、appcompat-v7库也不再支持那么久远的系统了，但是它们的名字却一直保留了下来，虽然它们现在的实际作用已经对不上当初命名的原因了。

那么很明显，Android团队也意识到这种命名已经非常不合适了，于是对这些API的架构进行了一次重新的划分，推出了AndroidX。因此，AndroidX本质上其实就是对Android Support Library进行的一次升级，升级内容主要在于以下两个方面。

1. 包名。之前Android Support Library中的API，它们的包名都是在android.support.*下面的，而AndroidX库中所有API的包名都变成了在androidx.*下面。这是一个很大的变化，意味着以后凡是android.*包下面的API都是随着Android操作系统发布的，而androidx.*包下面的API都是随着扩展库发布的，这些API基本不会依赖于操作系统的具体版本。

1. 命名规则。吸取了之前命名规则的弊端，AndroidX所有库的命名规则里都不会再包含具体操作系统API的版本号了。比如，像appcompat-v7库，在AndroidX中就变成了appcompat库。

一个AndroidX完整的依赖库格式如下所示：

#+BEGIN_SRC groovy
implementation 'androidx.appcompat:appcompat:1.0.2'
#+END_SRC

了解了AndroidX是什么之后，现在你应该放轻松了吧？它其实并不是什么全新的东西，而是对Android Support Library的一次升级。因此，AndroidX上手起来也没有任何困难的地方，比如之前你经常使用的RecyclerView、ViewPager等等库，在AndroidX中都会有一个对应的版本，只要改一下包名就可以完全无缝使用，用法方面基本上都没有任何的变化。

但是有一点需要注意，AndroidX和Android Support Library中的库是非常不建议混合在一起使用的，因为它们可能会产生很多不兼容的问题。最好的做法是，要么全部使用AndroidX中的库，要么全部使用Android Support Library中的库。

而现在Android团队官方的态度也很明确，未来都会为AndroidX为主，Android Support Library已经不再建议使用，并会慢慢停止维护。另外，从Android Studio 3.4.2开始，我发现新建的项目已经强制勾选使用AndroidX架构了。