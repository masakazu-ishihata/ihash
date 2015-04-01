# ihash

自作 hash ライブラリ。（しょぼい）   
内部で [ilist][ilist], [istring][istring] を使ってます。  
iHoge の i は ishihata の i です。（重要）   

[ilist]: https://github.com/masakazu-ishihata/ilist "ilist"
[iutil]: https://github.com/masakazu-ishihata/istring "istring"

## methods
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

### list

    void ihash_get_keys(ihash *_ih, ilist *_l);
    void ihash_get_vals(ihash *_ih, ilist *_l);
    void ihash_get_pairs(ihash *_ih, ilist *_l);

list _l に ih 内の key/val/pair を格納する。  
list の中身は key/val/pair へのポインタである。  
list は自分で new/free すること。

### show

    void ihash_show(ihash *_ih, FILE *fp);

ih 内の要素の key の一覧を fp に出力する。

## 使い方

ご想像にお任せします！
