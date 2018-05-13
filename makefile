#ファイル定義
#TODO:ファイルを定義する。
TARGET=a.out
OBJS=main.o swtch.o

#ビルドの過程で生成されるファイル
PRODUCTS=*.o *.out

#コンパイラ設定
LIBDIR=
INCDIR=
LIBS=
CCFLAGS=

#インストール設定
INSTALLPATH=/usr/local/bin

#ツール定義
CC=gcc
AS=gcc -c
MAKE=make
RM=rm -f
DEBUGGER=gdb


.PHONY:all clean rebuild
all: $(TARGET)
#ファイルの依存関係
#TODO:ファイルの依存関係を記述する。

#ターゲットの作成
$(TARGET):$(OBJS)
	$(CC) $^ $(LIBDIR) $(LIBS) -o $@

#サフィックスルール
.SUFFIXES:.c .o .s
.c.o:
	$(CC) $(CCFLAGS) $(INCDIR) $< -c
.s.o:
	$(AS) $<

#ユーティリティコマンド定義
rebuild:
	$(MAKE) clean
	$(MAKE)

clean:
	$(RM) $(PRODUCTS) $(TARGET)

run:all
	./$(TARGET)

debug:all
	$(DEBUGGER) $(TARGET)

#インストール&アンインストールユーティリティ
install:all
	$(CP) $(TARGET) $(INSTALLPATH)/$(TARGET)

uninstall:
	$(RM) $(INSTALLPATH)/$(TARGET)
