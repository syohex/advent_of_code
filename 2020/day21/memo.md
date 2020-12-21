# memo

## 解き方

サンプル入力

```
mxmxvkd kfcds sqjhc nhms (contains dairy, fish)
trh fvjkl sbzzf mxmxvkd (contains dairy)
sqjhc fvjkl (contains soy)
sqjhc mxmxvkd sbzzf (contains fish)
```

ステップ 1 すべての成分がすべてのアレルギー物質を持っているものとする

```
mxmxvkd : [dairy, fish, soy]
kfcds : [dairy, fish, soy]
sqjhc : [dairy, fish, soy]
nhms : [dairy, fish, soy]
trh : [dairy, fish, soy]
fvjkl : [dairy, fish, soy]
sbzzf : [dairy, fish, soy]
```

ステップ 2 各成分について各食べ物に含まれるアレルギーと照らし合わせ該当しないものを候補から消していく

例えば fvjklに注目する. 例えば 1行目を見たとき, そこに含まれない fvjklはこの食べ物のアレルギー物質を含まれないので dairy, fishをアレルギー物質候補から消す. fvjklについてすべての食べ物との照らし合わせを行ったあと fvjklの候補は soy だけになる. つまり fvjkl=soyが確定するので fvjklを除くすべての候補から soyを消す.

この作業をすべての成分について行い候補の絞り込みを行う

ステップ 3 各成分の候補が 0, 1になるまでステップ 2を繰り返す

結果として以下が得られる

```
fvjkl -> [soy]
kfcds -> []
mxmxvkd -> [dairy]
nhms -> []
sbzzf -> []
sqjhc -> [fish]
trh -> []
```

Part1は, 候補が 0つまりアレルギー物質を持たない成分が各食べ物に何回出現するかを数え上げればよい.
Part2は, アレルギー物質を持つ成分をアレルギー物質をキーとしてソートして結合すればよい
