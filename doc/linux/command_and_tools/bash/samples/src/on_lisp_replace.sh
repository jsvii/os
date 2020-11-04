#!/bin/zsh

# 不要删除，图灵社区抓取文件脚本
for file in *org; do

sed -i '
s/<p>//g
s/<\/p>//g
s/<ol>//g
s/<\/ol>//g
s/<ul>//g
s/<\/ul>//g
s/<\/li>//g
s/<li>/- /g
s/<\/h2>//g
s/<h2>/* /g
s/<\/h3>//g
s/<h3>/** /g
s/<\/h4>//g
s/<h4>/*** /g
s/<\/h5>//g
s/<h5>/**** /g
s/<hr\/>//g
s/<blockquote>/#+BEGIN_QUOTE/g
s/<\/blockquote>/#+END_QUOTE/g
s/<pre><code>/#+BEGIN_SRC common-lisp\n/g
s/<\/code><\/pre>/#+END_SRC/g
s/<em>/ ~/g
s/<\/em>/~ /g
s/&amp;/\&/g
s/&#39;/\'\''/g
s/&gt;/>/g
s/&lt;/</g
s/&quot;/"/g
s/ <code>/ ~/g
s/<code>/ ~/g
s/<\/code> /~ /g
s/<\/code>/~ /g
s/<strong>/*/g
s/<\/strong>/*/g
' $file
done
