FasdUAS 1.101.10   ��   ��    k             l     �� ��    D >--------------------------------------------------------------       	  l     �� 
��   
 D >--------------------------------------------------------------    	     l     �� ��    ' ! Enfuse droplet applescript 0.0.2         l     �� ��    %  Harry van der wolf 15 Mar 2008         l     �� ��    D >--------------------------------------------------------------         l     �� ��    D >--------------------------------------------------------------         i         I      �������� 0 
func_about 
FUNC_ABOUT��  ��    k     )       I    '��  
�� .sysodlogaskr        TEXT  b        !   b      " # " b      $ % $ b      & ' & b      ( ) ( b      * + * b      , - , b      . / . b      0 1 0 b      2 3 2 b      4 5 4 b     	 6 7 6 b      8 9 8 b      : ; : b      < = < m      > > 0 *Enfuse droplet 0.0.2 by Harry van der Wolf    = o    ��
�� 
ret  ; o    ��
�� 
ret  9 l 	   ?�� ? m     @ @ ; 5This (G)UI wrapper uses enfuse and align_image_stack    ��   7 l 	   A�� A m     B B 9 3to fuse your set of images with different exposure    ��   5 l 	 	 
 C�� C m   	 
 D D " settings to one fused image.   ��   3 o    ��
�� 
ret  1 o    ��
�� 
ret  / l 	   E�� E m     F F 1 +You can use this GUI either by starting it    ��   - l 	   G�� G m     H H * $manually and then select your images   ��   + l 	   I�� I o    ��
�� 
ret ��   ) o    ��
�� 
ret  ' m     J J  OR    % o    ��
�� 
ret  # o    ��
�� 
ret  ! l 	   K�� K m     L L = 7drag and drop your images onto this little application.   ��    �� M��
�� 
btns M J     # N N  O�� O m     ! P P  Ok   ��  ��     Q�� Q l  ( (������  ��  ��     R S R l     �� T��   T D >--------------------------------------------------------------    S  U V U l     �� W��   W D >--------------------------------------------------------------    V  X Y X i     Z [ Z I      ��������  0 func_ais_param FUNC_AIS_PARAM��  ��   [ k     C \ \  ] ^ ] I    A�� _ `
�� .sysodlogaskr        TEXT _ b     9 a b a b     7 c d c b     5 e f e b     3 g h g b     1 i j i b     / k l k b     - m n m b     + o p o b     ) q r q b     ' s t s b     % u v u b     # w x w b     ! y z y b      { | { b      } ~ } b       �  b      � � � b      � � � b      � � � b      � � � b      � � � b      � � � b      � � � b      � � � b     	 � � � b      � � � b      � � � b      � � � m      � � " align_image_stack parameters    � o    ��
�� 
ret  � o    ��
�� 
ret  � l 	   ��� � m     � � 6 0# default parameters already set by this script:   ��   � o    ��
�� 
ret  � l 	 	 
 ��� � m   	 
 � � - '-p file, output .pto => set to pano.pto   ��   � o    ��
�� 
ret  � l 	   ��� � m     � � , &-a prefix align images => set to fused   ��   � o    ��
�� 
ret  � o    ��
�� 
ret  � l 	   ��� � m     � � , &# other parameters (to be set by you):   ��   � o    ��
�� 
ret  � l 	   ��� � m     � � K E-e assume input images are full frame fish eye (default: rectilinear)   ��   � o    ��
�� 
ret  ~ l 	   ��� � m     � � [ U-t num    Remove all control points with an error higher than num pixels (default: 3)   ��   | o    ��
�� 
ret  z l 	    ��� � m      � � c ]-f HFOV   approximate horizontal field of view of input images, use if EXIF info not complete   ��   x o   ! "��
�� 
ret  v l 	 # $ ��� � m   # $ � � � �-m        Optimize field of view for all images, except for first. Useful for aligning focus stacks with slightly different magnification.   ��   t o   % &��
�� 
ret  r l 	 ' ( ��� � m   ' ( � � b \-c num    number of control points (per grid) to create between adjacent images (default: 8)   ��   p o   ) *��
�� 
ret  n l 	 + , ��� � m   + , � � ) #-l        Assume linear input files   ��   l o   - .��
�� 
ret  j l 	 / 0 ��� � m   / 0 � � J D-s scale  Scale down image by 2^scale (default: 1 [2x downsampling])   ��   h o   1 2��
�� 
ret  f l 	 3 4 ��� � m   3 4 � � � �-g gsize  Break image into a rectangular grid (gsize x gsize) and attempt to find 
             num control points in each section (default: 5 [5x5 grid] )   ��   d o   5 6��
�� 
ret  b l 
 7 8 ��� � o   7 8��
�� 
ret ��   ` �� ���
�� 
btns � J   : = � �  ��� � m   : ; � �  Ok   ��  ��   ^  ��� � l  B B������  ��  ��   Y  � � � l     �� ���   � D >--------------------------------------------------------------    �  � � � l     �� ���   � D >--------------------------------------------------------------    �  � � � i     � � � I      ��������  0 func_enf_param FUNC_ENF_PARAM��  ��   � I    ��� � �
�� .sysodlogaskr        TEXT � b     � � � � b     � � � � b     � � � � b     � � � � b     � � � � b     � � � � b     � � � � b     � � � � b     � � � � b     } � � � b     y � � � b     u � � � b     q � � � b     o � � � b     k � � � b     i � � � b     e � � � b     c � � � b     _ � � � b     ] � � � b     Y � � � b     W � � � b     S � � � b     Q � � � b     M � � � b     K � � � b     G � � � b     E � � � b     C � � � b     ? � � � b     ;   b     9 b     7 b     5 b     3	 b     1

 b     / b     - b     + b     ) b     ' b     % b     # b     ! b      b      b      !  b     "#" b     $%$ b     &'& b     ()( b     *+* b     ,-, b     ./. b     010 b     	232 b     454 b     676 b     898 m     :: # ==== enfuse, version 3.1 ====   9 o    ��
�� 
ret 7 l 	  ;��; m    <<  Common options:   ��  5 o    ��
�� 
ret 3 l 	  =��= m    >> ? 9 -l number              Number of levels to use (1 to 29)   ��  1 o   	 
��
�� 
ret / l 	  ?��? m    @@ 2 , -o filename            Write output to file   ��  - o    ��
�� 
ret + l 	  A��A m    BB < 6-w                     Blend across -180/+180 boundary   ��  ) o    ��
�� 
ret ' l 	  C��C m    DD A ;--compression=COMP     Set compression of the output image.   ��  % o    ��
�� 
ret # l 	  E��E m    FF ' !Valid values for compression are:   ��  ! o    ��
�� 
ret  l 	  G��G m    HH " For TIFF files: LZW, DEFLATE   ��   o    ��
�� 
ret  l 	   I��I m     JJ  For JPEG files: 0-100   ��   o   ! "��
�� 
ret  o   # $��
�� 
ret  l 	 % &K��K m   % &LL  Extended options:   ��   o   ' (��
�� 
ret  l 	 ) *M��M m   ) *NN B <-b kilobytes           Image cache block size (default=2MiB)   ��   o   + ,��
�� 
ret  l 	 - .O��O m   - .PP 9 3-c                     Use CIECAM02 to blend colors   ��   o   / 0��
�� 
ret 	 l 	 1 2Q��Q m   1 2RR T N-g                     Associated alpha hack for Gimp (ver. < 2) and Cinepaint   ��   o   3 4��
�� 
ret  l 	 5 6S��S m   5 6TT T N-f WIDTHxHEIGHT+x0+y0  Manually set the size and position of the output image.   ��   o   7 8��
�� 
ret  l 	 9 :U��U m   9 :VV 7 1Useful for cropped and shifted input TIFF images,   ��   � l 	 ; >W��W m   ; >XX % such as those produced by Nona.   ��   � l 	 ? BY��Y m   ? BZZ U O-m megabytes           Use this much memory before going to disk (default=1GiB)   ��   � o   C D��
�� 
ret  � o   E F��
�� 
ret  � l 	 G J[��[ m   G J\\  Fusion options:   ��   � o   K L��
�� 
ret  � l 	 M P]��] m   M P^^ O I--wExposure=W          Weight given to well-exposed pixels (from 0 to 1).   ��   � o   Q R��
�� 
ret  � l 	 S V_��_ m   S V``  default value: 1.0   ��   � o   W X��
�� 
ret  � l 	 Y \a��a m   Y \bb S M--wSaturation=W        Weight given to highly-saturated pixels (from 0 to 1).   ��   � o   ] ^��
�� 
ret  � l 	 _ bc��c m   _ bdd  default value: 0.2   ��   � o   c d��
�� 
ret  � l 	 e he��e m   e hff P J--wContrast=W          Weight given to high-contrast pixels (from 0 to 1).   ��   � o   i j��
�� 
ret  � l 	 k ng��g m   k nhh  default value: 0   ��   � o   o p��
�� 
ret  � l 	 q ti��i m   q tjj L F--HardMask             Force hard blend masks (no averaging) on finest   ��   � l 	 u xk��k m   u xll 0 *scale. This is especially useful for focus   ��   � l 	 y |m�m m   y |nn 6 0stacks with thin and high contrast features such   �   � l 	 } �o�~o m   } �pp < 6as insect hairs etc, but will lead to increased noise.   �~   � o   � ��}
�} 
ret  � o   � ��|
�| 
ret  � l 	 � �q�{q m   � �rr  Expert options:   �{   � o   � ��z
�z 
ret  � l 	 � �s�ys m   � �tt E ?--ContrastWindowSize=s Window size for local contrast analysis.   �y   � o   � ��x
�x 
ret  � l 	 � �u�wu m   � �vv * $Default: 5, (must be bigger than 3).   �w   � l 
 � �w�vw o   � ��u
�u 
ret �v   � �tx�s
�t 
btnsx J   � �yy z�rz m   � �{{  Ok   �r  �s   � |}| l     �q~�q  ~ D >--------------------------------------------------------------   } � i    ��� I      �p��o�p 00 func_initialize_bp_pgb FUNC_Initialize_BP_PGB� ��� o      �n�n 0 titlebarmsg  � ��� o      �m�m 
0 topmsg  � ��l� o      �k�k 0 	bottommsg  �l  �o  � O     j��� k    i�� ��� I   	�j�i�h
�j .ascrnoop****      � ****�i  �h  � ��� r   
 ��� o   
 �g�g 0 titlebarmsg  � n      ��� 1    �f
�f 
titl� 4    �e�
�e 
cwin� m    �d�d � ��� I   �c�b�a
�c .miscactvnull��� ��� null�b  �a  � ��� I   !�`��_
�` .appSshoHnull���    obj � 4    �^�
�^ 
cwin� m    �]�] �_  � ��� O   " =��� O   ) <��� k   0 ;�� ��� r   0 5��� m   0 1�\
�\ boovtrue� 1   1 4�[
�[ 
indR� ��Z� I  6 ;�Y�X�W
�Y .coVSstaAnull���    obj �X  �W  �Z  � 4   ) -�V�
�V 
proI� m   + ,�U�U � n   " &��� 4   # &�T�
�T 
cwin� m   $ %�S�S � m   " #���null     ߀�� �_BP Progress Bar.app̘h����  ��g0�? �����Ș'̘�~����Ș��????@  alis    �  Macintosh HD               ����H+   �_BP Progress Bar.app                                             ��Ϳ���        ����  	                harryvanderwolf     ���      ��¸     �_  o   6Macintosh HD:Users:harryvanderwolf:BP Progress Bar.app  (  B P   P r o g r e s s   B a r . a p p    M a c i n t o s h   H D  )Users/harryvanderwolf/BP Progress Bar.app   /    ��  � ��� O  > S��� O  E R��� r   L Q��� o   L M�R�R 
0 topmsg  � 1   M P�Q
�Q 
conT� 4   E I�P�
�P 
texF� m   G H�O�O � n   > B��� 4   ? B�N�
�N 
cwin� m   @ A�M�M � m   > ?�� ��L� O  T i��� O  [ h��� r   b g��� o   b c�K�K 0 	bottommsg  � 1   c f�J
�J 
conT� 4   [ _�I�
�I 
texF� m   ] ^�H�H � n   T X��� 4   U X�G�
�G 
cwin� m   V W�F�F � m   T U��L  � m     �� ��� l     �E��E  � D >--------------------------------------------------------------   � ��� l     �D��D  � D >--------------------------------------------------------------   � ��� i    ��� I      �C�B�A�C $0 func_quit_bp_pgb FUNC_quit_BP_PGB�B  �A  � O    
��� I   	�@�?�>
�@ .aevtquitnull���    obj �?  �>  � m     �� ��� l     �=��=  � D >--------------------------------------------------------------   � ��� l     �<��<  � D >--------------------------------------------------------------   � ��� l     �;��;  �   function enfuse   � ��� i    ��� I      �:��9�: 0 func_enfuse FUNC_ENFUSE� ��� o      �8�8 0 
dropped_on  � ��7� o      �6�6 0 	imagelist 	ImageList�7  �9  � k    @�� ��� l     �5�4�5  �4  � ��� l     �3�2�3  �2  � ��� l     �1��1  � X R set enfuse_additional_parameters to "--wExposure=1 --wSaturation=1 --wContrast=1"   � ��� r     ��� m     ��      � o      �0�0  0 enfuse_additional_parameters  � ��� l   �/��/  � 6 0-- start images selection if dropped_on is false   � ��� Z    ����.�-� H    �� o    �,�, 0 
dropped_on  � k   	 ��� ��� r   	 ��� m   	 
��      � o      �+�+ 0 	imagelist 	ImageList� ��� r    &��� l 	  $��*� I   $�)�( 
�) .sysostdfalis    ��� null�(    �'
�' 
prmp l 	  �& m     + %Select the image files of your choice   �&   �%
�% 
dflc l 
  �$ l   �# I   �"	�!
�" .earsffdralis        afdr	 m    � 
�  afdrpdoc�!  �#  �$   �

� 
ftyp
 J      m     
 JPEG     m     
 TIFF    � m     	 PNG   �   �
� 
mlsl m    �
� boovtrue ��
� 
lfiv m     �
� boovfals�  �*  � o      �� 0 	theimages 	theImages�  X   ' �� k   9 �  r   9 D  n   9 B!"! 1   > B�
� 
strq" n   9 >#$# 1   : >�
� 
psxp$ o   9 :�� 0 oneimage OneImage  o      �� 0 oneimage OneImage %&% r   E L'(' n   E J)*) 1   F J�
� 
psxp* o   E F�� 0 oneimage OneImage( o      �� 0 testname  & +,+ l  M M�-�  - #  we need this extension later   , ./. Z   M v01�20 G   M f343 D   M V565 l  M R7�7 c   M R898 o   M N�� 0 testname  9 m   N Q�
� 
ctxt�  6 m   R U:: 
 .TIF   4 D   Y b;<; l  Y ^=�
= c   Y ^>?> o   Y Z�	�	 0 testname  ? m   Z ]�
� 
ctxt�
  < m   ^ a@@  .TIFF   1 r   i nABA m   i lCC 
 .tif   B o      �� 0 imgext ImgExt�  2 r   q vDED m   q tFF 
 .jpg   E o      �� 0 imgext ImgExt/ GHG l  w w���  �  H IJI l  w �KLK r   w �MNM n   w �OPO 1   | ��
� 
strqP n   w |QRQ 1   x |�
� 
psxpR o   w x�� 0 oneimage OneImageN o      � �  0 	imagepath 	ImagePathL * $should do it only once but who cares   J S��S r   � �TUT b   � �VWV b   � �XYX o   � ����� 0 	imagelist 	ImageListY o   � ����� 0 oneimage OneImageW m   � �ZZ      U o      ���� 0 	imagelist 	ImageList��  � 0 oneimage OneImage o   * +���� 0 	theimages 	theImages [��[ l  � ���\��  \   -- end of images selection   ��  �.  �-  � ]^] l  � �������  ��  ^ _`_ l  � ���a��  a u o Find the working dir of the program, otherwise it will default to $HOME where ais and enfuse are not available   ` bcb O  � �ded e   � �ff c   � �ghg n   � �iji m   � ���
�� 
cfolj l  � �k��k I  � ���l��
�� .earsffdralis        afdrl  f   � ���  ��  h m   � ���
�� 
utxte m   � �mm�null     � ��  �
Finder.appU  `�p''̘����Q  �g0�? �����Ș'̘$�~X����Ș��MACS   alis    r  Macintosh HD               ����H+    �
Finder.app                                                       ES��        ����  	                CoreServices    ���      ���      �  f  e  3Macintosh HD:System:Library:CoreServices:Finder.app    
 F i n d e r . a p p    M a c i n t o s h   H D  &System/Library/CoreServices/Finder.app  / ��  c non r   � �pqp n   � �rsr 1   � ���
�� 
psxps 1   � ���
�� 
rsltq o      ���� 0 
workingdir 
workingDiro tut l  � �������  ��  u vwv l  � ���x��  x 9 3-- start of requesting align_image_stack parameters   w yzy r   � �{|{ m   � �}}      | o      ����  0 align_image_stack_parameters  z ~~ r   � ���� m   � ��� 	 NOK   � o      ���� 0 	get_going   ��� V   �x��� k   �s�� ��� I  �*����
�� .sysodlogaskr        TEXT� b   ���� b   ���� b   ���� b   � ��� b   � ���� b   � ���� b   � ���� b   � ���� b   � ���� b   � ���� b   � ���� b   � ���� b   � ���� b   � ���� b   � ���� m   � ��� " align_image_stack parameters   � o   � ���
�� 
ret � o   � ���
�� 
ret � l 	 � ����� m   � ��� . (# default parameters set by this script:   ��  � o   � ���
�� 
ret � l 	 � ����� m   � ��� - '-p file, output .pto => set to pano.pto   ��  � o   � ���
�� 
ret � l 	 � ����� m   � ��� , &-a prefix align images => set to fused   ��  � o   � ���
�� 
ret � o   � ���
�� 
ret � l 	 � ����� m   � ��� + %# Other parameters can be set by you:   ��  � o   � ���
�� 
ret � l 	 � ����� m   � ��� 3 -(Click Show Parameters to show possibilities)   ��  � o   ��
�� 
ret � o  ��
�� 
ret � l 	���� m  �� . (Enter your additional parameters below.    ��  � ����
�� 
dtxt� l 
���� o  ����  0 align_image_stack_parameters  ��  � ����
�� 
btns� J  �� ��� m  ��  Show Parameters   � ��� m  �� 
 Quit   � ���� m  ��  Ok   ��  � �����
�� 
dflt� m  !$��  Ok   ��  � ��� r  +0��� 1  +.��
�� 
rslt� o      ���� 0 dialogresult dialogResult� ��� r  18��� n  16��� 1  26��
�� 
ttxt� o  12���� 0 dialogresult dialogResult� o      ����  0 align_image_stack_parameters  � ��� r  9@��� n  9>��� 1  :>��
�� 
bhit� o  9:���� 0 dialogresult dialogResult� o      ���� 
0 choice  � ��� Z AR������� =  AF��� o  AB���� 
0 choice  � m  BE��  Show Parameters   � I  IN��������  0 func_ais_param FUNC_AIS_PARAM��  ��  ��  ��  � ��� l Sa��� Z Sa������� =  SX��� o  ST���� 
0 choice  � m  TW�� 
 Quit   � L  []����  ��  ��  �   exit script immediately   � ���� Z bs������� =  bg��� o  bc���� 
0 choice  � m  cf��  Ok   � r  jo��� m  jm��  OK   � o      ���� 0 	get_going  ��  ��  ��  � =  � ���� o   � ����� 0 	get_going  � m   � ��� 	 NOK   � ��� l yy�����  � 6 0- end of requesting align_image_stack parameters   � ��� l yy������  ��  � ��� l yy�����  � P J delete possible previous fused intermediate images (normally overwritten�   � ��� l yy�����  � h b when there are more images from a previous run than need to be calculated, you get strange images   � ��� l yy�� ��     tell application "Finder"   �  Q  y��� l |� I |�����
�� .sysoexecTEXT���     TEXT b  |�	 b  |�

 m  | 	 rm     o  ����� 0 
workingdir 
workingDir	 m  ��  
fused*.tif   ��   B < use try to not display (error) dialog on non existing files    R      ������
�� .ascrerr ****      � ****��  ��  ��    l ������    end tell     l ��������  ��    l ��������  ��    l ������   7 1-- kick off align_image_stack and set barber pole     I  �������� 00 func_initialize_bp_pgb FUNC_Initialize_BP_PGB  m  ��  Running align_image_stack     m  ��   / )Align_image_stack is aligning your images    !��! m  ��"" / )This will take some time. Please wait....   ��  ��   #$# I ����%��
�� .sysoexecTEXT���     TEXT% b  ��&'& b  ��()( b  ��*+* b  ��,-, b  ��./. b  ��010 b  ��232 b  ��454 m  ��66 	 cd    5 o  ������ 0 
workingdir 
workingDir3 m  ��77  ;    1 o  ������ 0 
workingdir 
workingDir/ m  ��88  align_image_stack   - m  ��99  
 -a fused    + o  ������  0 align_image_stack_parameters  ) m  ��::      ' o  ������ 0 	imagelist 	ImageList��  $ ;<; I  ���������� $0 func_quit_bp_pgb FUNC_quit_BP_PGB��  ��  < =>= l ����?��  ? 5 /-- align_image_stack finished. stop barber pole   > @A@ l ��������  ��  A BCB l ����D��  D . (-- start of requesting enfuse parameters   C EFE r  ��GHG m  ��II 	 NOK   H o      ���� 0 	get_going  F JKJ V  �cLML k  �^NN OPO I ���QR
�� .sysodlogaskr        TEXTQ b  ��STS b  ��UVU b  ��WXW b  ��YZY b  ��[\[ b  ��]^] b  ��_`_ m  ��aa " Enfuse additional parameters   ` o  ����
�� 
ret ^ o  ����
�� 
ret \ l 	��b��b m  ��cc + %Here you can specify your parameters.   ��  Z o  ����
�� 
ret X l 	��d��d m  ��ee 3 -(Click Show Parameters to show possibilities)   ��  V o  ����
�� 
ret T l 
��f��f o  ����
�� 
ret ��  R ��gh
�� 
dtxtg o  ������  0 enfuse_additional_parameters  h ��ij
�� 
btnsi J  �	kk lml m  �nn  Show Parameters   m opo m  qq 
 Quit   p r��r m  ss  Ok   ��  j ��t��
�� 
dfltt m  uu  Ok   ��  P vwv r  xyx 1  ��
�� 
rslty o      ���� 0 dialogresult dialogResultw z{z r  #|}| n  !~~ 1  !��
�� 
ttxt o  ���� 0 dialogresult dialogResult} o      ����  0 enfuse_additional_parameters  { ��� r  $+��� n  $)��� 1  %)��
�� 
bhit� o  $%���� 0 dialogresult dialogResult� o      �� 
0 choice  � ��� Z ,=���~�}� =  ,1��� o  ,-�|�| 
0 choice  � m  -0��  Show Parameters   � I  49�{�z�y�{  0 func_enf_param FUNC_ENF_PARAM�z  �y  �~  �}  � ��� l >L��� Z >L���x�w� =  >C��� o  >?�v�v 
0 choice  � m  ?B�� 
 Quit   � L  FH�u�u  �x  �w  �   exit script immediately   � ��t� Z M^���s�r� =  MR��� o  MN�q�q 
0 choice  � m  NQ��  Ok   � r  UZ��� m  UX��  OK   � o      �p�p 0 	get_going  �s  �r  �t  M = ����� o  ���o�o 0 	get_going  � m  ���� 	 NOK   K ��� l dd�n��n  � + %- end of requesting enfuse parameters   � ��� l dd�m�l�m  �l  � ��� l dd�k��k  � A ;--Ask filename of fused image and where it should be stored   � ��� r  dy��� l dw��j� I dw�i�h�
�i .sysonwflfile    ��� null�h  � �g��
�g 
prmt� m  hk�� 2 ,Specify the filename of your new fused image   � �f��e
�f 
dflc� l lq��d� I lq�c��b
�c .earsffdralis        afdr� m  lm�a
�a afdrpdoc�b  �d  �e  �j  � o      �`�` 0 
newimgname 
NewImgName� ��� r  z���� n  z���� 1  ��_
�_ 
strq� n  z��� 1  {�^
�^ 
psxp� o  z{�]�] 0 
newimgname 
NewImgName� o      �\�\ 0 newimage NewImage� ��� r  ����� n  ����� 1  ���[
�[ 
psxp� o  ���Z�Z 0 
newimgname 
NewImgName� o      �Y�Y 0 testname  � ��� l ���X��X  � H B file extensions are not treated case-sensitive within applescript   � ��� Z  �����W�� G  ����� G  ����� G  ����� D  ����� l ����V� c  ����� o  ���U�U 0 testname  � m  ���T
�T 
ctxt�V  � m  ���� 
 .TIF   � D  ����� l ����S� c  ����� o  ���R�R 0 testname  � m  ���Q
�Q 
ctxt�S  � m  ���� 
 .JPG   � D  ����� l ����P� c  ����� o  ���O�O 0 testname  � m  ���N
�N 
ctxt�P  � m  ����  .TIFF   � D  ����� l ����M� c  ����� o  ���L�L 0 testname  � m  ���K
�K 
ctxt�M  � m  ����  .JPEG   � k  ���� ��� r  ����� n  ����� 1  ���J
�J 
strq� n  ����� 1  ���I
�I 
psxp� o  ���H�H 0 testname  � o      �G�G 0 newimage NewImage� ��F� l ���E��E  �  display dialog NewImage   �F  �W  � k  ���� ��� l ���D��D  � + % No image extension, so make it a tif   � ��� r  ����� b  ����� o  ���C�C 0 testname  � m  ���� 
 .tif   � o      �B�B 0 testname  �    r  �� n  �� 1  ���A
�A 
strq n  �� 1  ���@
�@ 
psxp o  ���?�? 0 testname   o      �>�> 0 newimage NewImage �= l ���<	�<  	  display dialog NewImage   �=  � 

 l ���;�;   , &-- kick off enfuse and set barber pole     I  ���:�9�: 00 func_initialize_bp_pgb FUNC_Initialize_BP_PGB  m  ��  Running enfuse     m  �� # enfuse is merging your images    �8 m  �� / )This will take some time. Please wait....   �8  �9    I �$�7�6
�7 .sysoexecTEXT���     TEXT b  �  b  � b  �  b  �!"! b  �#$# b  �%&% b  �'(' b  �
)*) b  �+,+ b  �-.- m  �// 	 cd    . o  �5�5 0 
workingdir 
workingDir, m  00  ;    * o  	�4�4 0 
workingdir 
workingDir( m  
11  enfuse    & o  �3�3  0 enfuse_additional_parameters  $ m  22 
  -o    " o  �2�2 0 newimage NewImage  m  33       o  �1�1 0 
workingdir 
workingDir m  44  
fused*.tif   �6   565 I  %*�0�/�.�0 $0 func_quit_bp_pgb FUNC_quit_BP_PGB�/  �.  6 787 l ++�-9�-  9 * $-- enfuse finished. stop barber pole   8 :;: l ++�,�+�,  �+  ; <=< l ++�*>�*  > ' !-- Show fused image to the public   = ?@? l ++�)A�)  A L F Stupid preview application is not scriptable so we need to shell out.   @ BCB l ++�(D�(  D l f Next to that there is another stupidity: It's called Preview.app on Tiger and preview.app on Leopard.   C EFE I +4�'G�&
�' .sysoexecTEXT���     TEXTG b  +0HIH m  +.JJ % open /Applications/Preview.app    I o  ./�%�% 0 newimage NewImage�&  F KLK I 5>�$M�#
�$ .sysoexecTEXT���     TEXTM b  5:NON m  58PP % open /Applications/preview.app    O o  89�"�" 0 newimage NewImage�#  L Q�!Q l ??� ��   �  �!  � RSR l     �T�  T   end of function enfuse   S UVU l     �W�  W D >--------------------------------------------------------------   V XYX l     �Z�  Z D >--------------------------------------------------------------   Y [\[ l     ���  �  \ ]^] l     ���  �  ^ _`_ l     �a�  a   Main part of script   ` bcb l     �d�  d   Here does it all start   c efe l     �g�  g 1 + Define some (initial) variables/properties   f hih p    jj ��� 0 	imagelist 	ImageList�  i klk l    m�m r     non m     pp      o o      �� 0 	imagelist 	ImageList�  l qrq l     ���  �  r sts l     �u�  u 6 0 "open" handler triggered by drag'n'drop launch.   t vwv l     �x�  x 6 0 This parts starts when a user drops files on it   w yzy i    {|{ I     �}�
� .aevtodocnull  �    alis} o      �
�
 0 finderobjects finderObjects�  | k     �~~ � p      �� �	��	 0 
hugin_path 
HUGIN_PATH�  � ��� l     ���  � 1 +set HUGIN_PATH to "/Applications/Hugin.app"   � ��� r     ��� m     ��      � o      �� 0 
hugin_path 
HUGIN_PATH� ��� r    ��� m    ��      � o      �� 0 	imagelist 	ImageList� ��� X    ����� l   ���� k    ��� ��� l   ���  � 8 2set pipo to quoted form of POSIX path of i as text   � ��� l   ���  � 4 .display dialog pipo -- show file/folder's info   � ��� Z    z���� � =   !��� n    ��� 1    ��
�� 
asdr� l   ���� I   �����
�� .sysonfo4asfe        file� o    ���� 0 oneimage OneImage��  ��  � m     ��
�� boovtrue� l  $ v��� X   $ v����� k   4 q�� ��� r   4 ;��� n   4 9��� 1   7 9��
�� 
strq� n   4 7��� 1   5 7��
�� 
psxp� o   4 5���� 0 oneimage OneImage� o      ���� 0 oneimage OneImage� ��� r   < A��� n   < ?��� 1   = ?��
�� 
psxp� o   < =���� 0 
newimgname 
NewImgName� o      ���� 0 testname  � ��� l  B B�����  � #  we need this extension later   � ��� Z   B _������ G   B Q��� D   B G��� l  B E���� c   B E��� o   B C���� 0 testname  � m   C D��
�� 
ctxt��  � m   E F�� 
 .TIF   � D   J O��� l  J M���� c   J M��� o   J K���� 0 testname  � m   K L��
�� 
ctxt��  � m   M N��  .TIFF   � r   T W��� m   T U�� 
 .tif   � o      ���� 0 imgext ImgExt��  � r   Z _��� m   Z ]�� 
 .jpg   � o      ���� 0 imgext ImgExt� ��� l  ` g��� r   ` g��� n   ` e��� 1   c e��
�� 
strq� n   ` c��� 1   a c��
�� 
psxp� o   ` a���� 0 oneimage OneImage� o      ���� 0 	imagepath 	ImagePath� * $should do it only once but who cares   � ���� r   h q��� b   h o��� b   h k��� o   h i���� 0 	imagelist 	ImageList� o   i j���� 0 oneimage OneImage� m   k n��      � o      ���� 0 	imagelist 	ImageList��  �� 0 oneimage OneImage� o   ' (���� 0 finderobjects finderObjects� $  process folder's contents too   �  �   � ��� l  { {�����  � - '		repeat with OneImage in finderObjects   � ��� r   { ���� n   { ���� 1   ~ ���
�� 
strq� n   { ~��� 1   | ~��
�� 
psxp� o   { |���� 0 oneimage OneImage� o      ���� 0 oneimage OneImage� ��� l  � ���� r   � ���� n   � ���� 1   � ���
�� 
strq� n   � ���� 1   � ���
�� 
psxp� o   � ����� 0 oneimage OneImage� o      ���� 0 	imagepath 	ImagePath� * $should do it only once but who cares   � ��� r   � ���� b   � �   b   � � o   � ����� 0 	imagelist 	ImageList o   � ����� 0 oneimage OneImage m   � �      � o      ���� 0 	imagelist 	ImageList� �� l  � �����    		end repeat   ��  � 1 + in case multiple objects dropped on applet   � 0 oneimage OneImage� l   �� o    ���� 0 finderobjects finderObjects��  � 	 l  � ���
��  
   run function enfuse   	  r   � � m   � ���
�� boovtrue o      ���� 0 
dropped_on   �� I   � ������� 0 func_enfuse FUNC_ENFUSE  o   � ����� 0 
dropped_on   �� o   � ����� 0 	imagelist 	ImageList��  ��  ��  z  l     ����      end of drag'n'drop launch     l     ������  ��    l     ����   U O This is the part that starts when user opens application by double-clicking it     l     ������  ��    l    ��  r    !"! m    ##      " o      ���� 0 	imagelist 	ImageList��   $%$ l     ��&��  & !  ask user what he/she wants   % '(' l   )��) r    *+* m    	��
�� boovfals+ o      ���� 0 quit_app  ��  ( ,-, l   Y.��. W    Y/0/ k    T11 232 I   !��45
�� .sysodlogaskr        TEXT4 m    66   Click Enfuse to get going!   5 ��78
�� 
btns7 J    99 :;: m    <<  about   ; =>= m    ?? 
 quit   > @��@ m    AA  Enfuse   ��  8 ��B��
�� 
dfltB m    CC  Enfuse   ��  3 DED r   " 'FGF n   " %HIH 1   # %��
�� 
bhitI 1   " #��
�� 
rsltG o      ���� 
0 choice  E J��J Z   ( TKLMNK =   ( -OPO o   ( )���� 
0 choice  P m   ) ,QQ  Enfuse   L k   0 ?RR STS r   0 5UVU m   0 1��
�� boovfalsV o      ���� 0 
dropped_on  T W��W I   6 ?��X���� 0 func_enfuse FUNC_ENFUSEX YZY o   7 :���� 0 
dropped_on  Z [��[ o   : ;���� 0 	imagelist 	ImageList��  ��  ��  M \]\ =   B G^_^ o   B C���� 
0 choice  _ m   C F``  about   ] a��a I   J O�������� 0 
func_about 
FUNC_ABOUT��  ��  ��  N l  R Tbcb L   R T����  c ) #user has chosen quit so quit script   ��  0 =   ded o    ���� 0 quit_app  e m    ��
�� boovtrue��  - f��f l  Z `g��g I  Z `��h��
�� .aevtoappnull  �   � ****h  ;   Z \��  ��  ��       
��ijklmnopq��  i ������������������ 0 
func_about 
FUNC_ABOUT��  0 func_ais_param FUNC_AIS_PARAM��  0 func_enf_param FUNC_ENF_PARAM�� 00 func_initialize_bp_pgb FUNC_Initialize_BP_PGB�� $0 func_quit_bp_pgb FUNC_quit_BP_PGB�� 0 func_enfuse FUNC_ENFUSE
�� .aevtodocnull  �    alis
�� .aevtoappnull  �   � ****j �� ����rs���� 0 
func_about 
FUNC_ABOUT��  ��  r  s  >�� @ B D F H J L�� P��
�� 
ret 
�� 
btns
�� .sysodlogaskr        TEXT�� *��%�%�%�%�%�%�%�%�%�%�%�%�%�%�%��kvl OPk �� [����tu����  0 func_ais_param FUNC_AIS_PARAM��  ��  t  u  ��� � � � � � � � � � � � ��� ���
�� 
ret 
�� 
btns
�� .sysodlogaskr        TEXT�� D��%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%��kvl OPl �� �����vw����  0 func_enf_param FUNC_ENF_PARAM��  ��  v  w #:�<>@BDFHJLNPRTVXZ\^`bdfhjlnprtv�~{�}
� 
ret 
�~ 
btns
�} .sysodlogaskr        TEXT�� ���%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%�%a %a %�%�%a %�%a %�%a %�%a %�%a %�%a %�%a %�%a %a %a %a %�%�%a %�%a %�%a %�%a  a !kvl "m �|��{�zxy�y�| 00 func_initialize_bp_pgb FUNC_Initialize_BP_PGB�{ �xz�x z  �w�v�u�w 0 titlebarmsg  �v 
0 topmsg  �u 0 	bottommsg  �z  x �t�s�r�t 0 titlebarmsg  �s 
0 topmsg  �r 0 	bottommsg  y ��q�p�o�n�m�l�k�j�i�h
�q .ascrnoop****      � ****
�p 
cwin
�o 
titl
�n .miscactvnull��� ��� null
�m .appSshoHnull���    obj 
�l 
proI
�k 
indR
�j .coVSstaAnull���    obj 
�i 
texF
�h 
conT�y k� g*j O�*�k/�,FO*j O*�k/j O��k/ *�k/ e*�,FO*j UUO��k/ *�k/ �*�,FUUO��k/ *�l/ �*�,FUUUn �g��f�e{|�d�g $0 func_quit_bp_pgb FUNC_quit_BP_PGB�f  �e  {  | ��c
�c .aevtquitnull���    obj �d � *j Uo �b��a�`}~�_�b 0 func_enfuse FUNC_ENFUSE�a �^�^   �]�\�] 0 
dropped_on  �\ 0 	imagelist 	ImageList�`  } �[�Z�Y�X�W�V�U�T�S�R�Q�P�O�N�M�[ 0 
dropped_on  �Z 0 	imagelist 	ImageList�Y  0 enfuse_additional_parameters  �X 0 	theimages 	theImages�W 0 oneimage OneImage�V 0 testname  �U 0 imgext ImgExt�T 0 	imagepath 	ImagePath�S 0 
workingdir 
workingDir�R  0 align_image_stack_parameters  �Q 0 	get_going  �P 0 dialogresult dialogResult�O 
0 choice  �N 0 
newimgname 
NewImgName�M 0 newimage NewImage~ k���L�K�J�I�H�G�F�E�D�C�B�A�@�?�>:@�=CFZm�<�;�:}����9�������8�7����6��5�4�3�2��1����0�/�. "�-6789:�,I�acenqsu��+����*��)�(�����/01234JP
�L 
prmp
�K 
dflc
�J afdrpdoc
�I .earsffdralis        afdr
�H 
ftyp
�G 
mlsl
�F 
lfiv�E 

�D .sysostdfalis    ��� null
�C 
kocl
�B 
cobj
�A .corecnte****       ****
�@ 
psxp
�? 
strq
�> 
ctxt
�= 
bool
�< 
cfol
�; 
utxt
�: 
rslt
�9 
ret 
�8 
dtxt
�7 
btns
�6 
dflt�5 
�4 .sysodlogaskr        TEXT
�3 
ttxt
�2 
bhit�1  0 func_ais_param FUNC_AIS_PARAM
�0 .sysoexecTEXT���     TEXT�/  �.  �- 00 func_initialize_bp_pgb FUNC_Initialize_BP_PGB�, $0 func_quit_bp_pgb FUNC_quit_BP_PGB�+  0 func_enf_param FUNC_ENF_PARAM
�* 
prmt�) 
�( .sysonwflfile    ��� null�_A�E�O� ��E�O*����j ����mv�e�f� E�O i�[�a l kh �a ,a ,E�O�a ,E�O�a &a 
 �a &a a & 
a E�Y a E�O�a ,a ,E�O��%a %E�[OY��OPY hOa  )j a ,a &UO_ a ,E�Oa E�Oa  E�O �h�a ! a "_ #%_ #%a $%_ #%a %%_ #%a &%_ #%_ #%a '%_ #%a (%_ #%_ #%a )%a *�a +a ,a -a .mva /a 0a 1 2O_ E�O�a 3,E�O�a 4,E�O�a 5  
*j+ 6Y hO�a 7  hY hO�a 8  
a 9E�Y h[OY�QO a :�%a ;%j <W X = >hO*a ?a @a Am+ BOa C�%a D%�%a E%a F%�%a G%�%j <O*j+ HOa IE�O �h�a J a K_ #%_ #%a L%_ #%a M%_ #%_ #%a *�a +a Na Oa Pmva /a Qa 1 2O_ E�O�a 3,E�O�a 4,E�O�a R  
*j+ SY hO�a T  hY hO�a U  
a VE�Y h[OY�qO*a Wa X��j a Y ZE�O�a ,a ,E�O�a ,E�O�a &a [
 �a &a \a &
 �a &a ]a &
 �a &a ^a & �a ,a ,E�OPY �a _%E�O�a ,a ,E�OPO*a `a aa bm+ BOa c�%a d%�%a e%�%a f%�%a g%�%a h%j <O*j+ HOa i�%j <Oa j�%j <OPp �'|�&�%���$
�' .aevtodocnull  �    alis�& 0 finderobjects finderObjects�%  � �#�"�!� ����# 0 finderobjects finderObjects�" 0 oneimage OneImage�! 0 
newimgname 
NewImgName�  0 testname  � 0 imgext ImgExt� 0 	imagepath 	ImagePath� 0 
dropped_on  � �������������������� 0 
hugin_path 
HUGIN_PATH� 0 	imagelist 	ImageList
� 
kocl
� 
cobj
� .corecnte****       ****
� .sysonfo4asfe        file
� 
asdr
� 
psxp
� 
strq
� 
ctxt
� 
bool� 0 func_enfuse FUNC_ENFUSE�$ ��E�O�E�O ��[��l kh �j �,e  W Q�[��l kh ��,�,E�O��,E�O��&�
 	��&��& �E�Y a E�O��,�,E�Oá%a %E�[OY��Y hO��,�,E�O��,�,E�Oá%a %E�OP[OY�|OeE�O*��l+ q �������
� .aevtoappnull  �   � ****� k     `�� k�� �� '�� ,�� f��  �  �  �  � p�#�
6�	<?A�C�����Q��`� ��� 0 	imagelist 	ImageList�
 0 quit_app  
�	 
btns
� 
dflt� 
� .sysodlogaskr        TEXT
� 
rslt
� 
bhit� 
0 choice  � 0 
dropped_on  � 0 func_enfuse FUNC_ENFUSE�  0 
func_about 
FUNC_ABOUT
�� .aevtoappnull  �   � ****� a�E�O�E�OfE�O Lh�e �����mv��� O��,E�O�a   fE` O*_ �l+ Y �a   
*j+ Y h[OY��O*6j ascr  ��ޭ