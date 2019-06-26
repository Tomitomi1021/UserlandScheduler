#ファイル定義
#TODO:ファイルを定義する。
TARGET=a.out
OBJS=main.o swtchC.o swtchAsm.o

#ビルドの過程で生成されるファイル
PRODUCTS=*.o *.out

#コンパイラ設定
LINKOPTION=
COMPILEOPTION=
LIBS=

#インストール設定
INSTALLPATH=/usr/local/bin

#ツール定義
MAKE=make
RM=rm -f
DEBUGGER=gdb


.PHONY:all clean rebuild
all: $(TARGET)
#ファイルの依存関係

#ターゲットの作成
$(TARGET):$(OBJS)
	gcc $(LINKOPTION) $^ $(LIBS) -o $@

#サフィックスルール
.SUFFIXES:.c .o .s
.c.o:
	gcc $(COMPILEOPTION) $< -c
.s.o:
	nasm -f elf64 -o $@ $<

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
