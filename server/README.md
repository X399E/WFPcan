# サーバーの中で動作させるプログラム  
## 動作環境    
| ソフトウェア | バージョン |
|-------------|-----------|
| OS          | Ubuntu Server 22.04 |
| Node.js     | 20.18.3 |
| MySQL       | 8.4.0 |

## 環境設定  
### サーバー側でする設定(一から作る場合)
#### パッケージのインストールいろいろ
~~~
sudo apt install -y nodejs npm mySQL
cp server/server.js ~/
cd server
~~~
#### 権限とかネットワークの設定とかいろいろ
~~~
どわすれしてました
明日書きます
~~~
#### mySQLを起動してテーブルを作るコマンド
~~~
CREATE USER 'ユーザー名'@'localhost' IDENTIFIED BY 'パスワード';
GRANT ALL PRIVILEGES ON * . * TO 'ユーザー名'@'localhost';
FLUSH PRIVILEGES;
CREATE TABLE val(id varchar(20),start varchar(15),end varchar(15),title varchar(10),value varchar(30));
CREATE TABLE user(name varchar(20),token varchar(129),password(129),time int);
CREATE TABLE current(id varchar(20),value varchar(15));
~~~
### サーバーを起動するたびに打つべきコマンド
~~~
node server.js
~~~
## その他の説明
sakuraVPSおすすめ