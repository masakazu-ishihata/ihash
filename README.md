# ihash

自作 hash ライブラリ。（しょぼい）   
内部で [ilist][ilist], [istring][istring] を使ってます。  
iHoge の i は ishihata の i です。（重要）   

[ilist]: https://github.com/masakazu-ishihata/ilist "ilist"
[iutil]: https://github.com/masakazu-ishihata/istring "istring"

## methods
### new/clear/free/realloc

    ihash *ihash_new(size_t _size);

ihash インスタンスを生成する。  
ただし _size はテーブルサイズ。

    ihash *ihash_clear(ihash *_ih);

_ih を空にする。  
ただし登録した値は free しない。

    ihash *ihash_clear_func(ihash *_ih, void (*free_func)(void *)));

_ih を空にする。  
同時に登録した値も free_func により free される。

    void ihash_free(ihash *_ih);

_ih を free する。   
ただし登録した値は free しない。

    void ihash_free_func(ihash *_ih, void (*free_func)(void *));

_ih を free する。   
同時に登録した値も free_func により free される。

    void ihash_realloc(ihash *_ih, size_t _size);

_ih のサイズを _size に変更する。  

    size_t ihash_size(ihash *_ih)
    size_t ihash_item(ihash *_ih)

_ih のサイズ/の要素数を返す。

### get/set/delete

    void *ihash_get(ihash *_ih, const char *_key);

key が _key である要素を探し、その値を返す。    
見つからなければ NULL を返す。

    int ihash_set(ihash *_ih, const char *_key, void *_val);

[key, val] を要素として登録する。  
ただしく登録されれば 0 を、そうでなければ -1 を返す。  
占有率が 2/3 を越えたらサイズを 3/2 に自動的に拡大する。  

    void *ihash_delete(ihash *_ih, const char *_key);

key が _key である要素探し、そのレコードを削除する。  
削除に成功した場合、削除された val のポインタを返す。  
val は free されない。


### list

    void ihash_keys(ihash *_ih, ilist *_l);
    void ihash_vals(ihash *_ih, ilist *_l);
    void ihash_pairs(ihash *_ih, ilist *_l);

list _l に ih 内の key/val/pair を格納する。  
list の中身は key/val/pair へのポインタである。  
list は自分で new/free すること。

### show

    void ihash_show(ihash *_ih, FILE *fp);

ih 内の要素の key の一覧を fp に出力する。

## 使い方

ご想像にお任せします！
