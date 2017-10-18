commit a0e59865aa79e4496075f2200802d2badb0e18ab
Author: Simon <simon@simon-X55C.lan>
Date:   Sat Jan 10 18:22:25 2015 +0100

    with first c file

diff --git a/init.c b/init.c
new file mode 100644
index 0000000..bf9fb2c
--- /dev/null
+++ b/init.c
@@ -0,0 +1,9 @@
+void init()	{
+	char *video = (char*) 0xb8000;
+	char Message[] = "Operating System - VIENNICE";
+	int i;
+	for(i=0; Message[i]!=0x00; i++)	{
+		video[2*i]=Message[i];
+		video[2*i+1]=0x07; //grau auf schwarzem Hintergrund!
+	}
+}
