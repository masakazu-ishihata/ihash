# iHash
自作 hash ライブラリ。（しょぼい）   
ihash の i は ishihata の i です。（重要）   

## Methods
### new/free

    ihash *ihash_new(size_t _size);

ihash インスタンスを生成する。  
ただし _size はテーブルサイズ。

    void ihash_free(ihash *_ih);

_ih を free する。   
ただし登録した値は free しない。

    void ihash_free_func(ihash *_ih, void (*free_func)(void *));

_ih を free する。   
同時に登録した値も free_func により free される。


### get/set
    
    void *ihash_get(ihash *_ih, const char *_key);

key が _key である要素を探し、その値を返す。    
見つからなければ NULL を返す。

    int ihash_set(ihash *_ih, const char *_key, void *_val);

[key, val] を要素として登録する。  
ただしく登録されれば 0 を、そうでなければ -1 を返す。

### show

    void ihash_show(ihash *_ih, FILE *fp);

ih 内の要素の key の一覧を fp に出力する。

## 使い方

ご想像にお任せします！
