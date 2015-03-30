# Revisao bibliografica #

**1. [Edição de Vídeo](EdicaoVideo.md)
  1. [Sistemas de vídeo](SistemaVideo.md)
  1. [Sistemas de cor](SistemaCor.md)
  1. [Ritmo Visual](RitmoVisual.md)
  1. Processamento de Baixo Nível
    1. [Filtragem Espacial](FiltragemEspacial.md)
    1. [Detecção de Bordas](Borda.md)
  1. Processamento de Médio Nível
    1. [Morfologia Matemtica](Morfologia.md)
    1. [Segmentação de Imagens](Segmentacao.md)
  1. Processamento de Alto Nível
    1. [Métodos de casamento de padrões](CasamentoDePadros.md)
    1. [Métodos Baseados em Entropia](Entropia.md)
  1. [Detecção de Transições](DeteccaoTransicao.md)**


# Trabalhos relacionados #

| **Nome**   | **Trabalho 1**      | **Trabalho 2**            | **Trabalho 3** |
|:-----------|:--------------------|:--------------------------|:---------------|
| mizu     | [cr2033](cr2033.md)          | [Conf\_02\_SIBGRAPI\_Silvio](Conf_02_SIBGRAPI_Silvio.md) | [ngo99detection](ngo99detection.md)  |
| fabricio | [Journal\_04\_JASP](Journal_04_JASP.md) | [297\_304](297_304.md)                 | [ZMM\_MM95](ZMM_MM95.md)        |
| ivan     | [KrisAthens](KrisAthens.md)      | [guimarae](guimarae.md)                | [bertrand97image](bertrand97image.md) |
| mauricio | [jamil](jamil.md)           | [Simoes](Simoes.md)                  | [a\_directional\_and\_parametrized](a_directional_and_parametrized.md) |

# Trabalhos relacionados 2 #


| **Nome**   | **Trabalho 1**      | **Trabalho 2**            | **Trabalho 3** |
|:-----------|:--------------------|:--------------------------|:---------------|
| mizu                | [Journal\_03\_prl\_jamil](Journal_03_prl_jamil.md) | [Movie\_scene\_segmentation\_using\_background\_information](Movie_scene_segmentation_using_background_information.md) (Não Serve) | [Circular\_discontinuities\_detection](Circular_discontinuities_detection.md) (Não Serve) |
| fabricio            | [A\_review\_of\_real\_time\_segmentation](A_review_of_real_time_segmentation.md) | [Journal\_00\_RITA](Journal_00_RITA.md) (nao serve) | [A\_new\_cut\_detection\_algorithm](A_new_cut_detection_algorithm.md) |
| ivan                | [A\_fuzzy\_logic\_approach\_for\_detection](A_fuzzy_logic_approach_for_detection.md) | [An\_efficient\_method\_for\_scene\_cut\_detection](An_efficient_method_for_scene_cut_detection.md) | [Content\_based\_indexing\_and\_retrieval\_of\_TV\_news](Content_based_indexing_and_retrieval_of_TV_news.md) (Bizarro n serve)|
| mauricio            | [Indexing\_for\_reuse\_of\_TV\_news\_shots](Indexing_for_reuse_of_TV_news_shots.md) | [Object\_based\_video\_retrieval\_with\_local\_region\_tracking](Object_based_video_retrieval_with_local_region_tracking.md) | [Temporal\_video\_segmentation\_and\_classification\_of\_edit\_effects](Temporal_video_segmentation_and_classification_of_edit_effects.md) |


# sort #
```
[fabriciols@linux01 fabriciols]$ cat sort.sh
cp -p journal_temp journal

while read who ; do
        NUM=0
        ALL=""

        while [ $NUM -ne 3 ] ; do
                RAND=$(expr $RANDOM % 10)
                FIND=$(sed -n ${RAND}p journal)
                if [ ! -z "$FIND" ] ; then
                        grep -v "$FIND" journal > journal.2
                        mv journal.2 journal
                        ((NUM++))
                        FIND=$( echo $FIND | sed s/.pdf//g )
                        FIND=$( echo $FIND | sed "s/[ -]/_/g" )
                        ALL="${ALL} || [$FIND]"
                fi
        done

        printf "|| %-18s %s ||\n" $who "$ALL"

done <<EOF
mizu
fabricio
ivan
mauricio
EOF
```

# Referencias #


---

[Link](http://code.google.com/p/m-fit/downloads/detail?name=Journal_04_JASP.pdf&can=2&q=#makechanges)
GUIMARÃES, S. J. F. ; LEITE, Neucimar Jerônimo ; COUPRIE, Michel ; ARAÚJO, Arnaldo de Albuquerque .**Video transition sharpening based on flat zone analysis. In: Non-linear Signal Image Processing, 2003**, Grado. EURASIP NSIP 2003, 2003.
Palavras-chave: Video transition; Morfologia Matemática; Segmentação do Vídeo; Video Digital.

---

[Link](http://m-fit.googlecode.com/files/cr2033.pdf)
GUIMARÃES, S. J. F. ; LEITE, Neucimar Jerônimo ; COUPRIE, Michel ; ARAÚJO, Arnaldo de Albuquerque .**An approach to detect video transitions based on mathematical morphology. In: International Conference on Image Processing, 2003**, Barcelone. IEEE ICIP 2003, 2003. v. III. p. 1021-1024.

---

[Link](http://m-fit.googlecode.com/files/Conf_02_SIBGRAPI_Silvio.pdf)
GUIMARÃES, S. J. F. ; LEITE, Neucimar Jerônimo ; COUPRIE, Michel ; ARAÚJO, Arnaldo de Albuquerque .**A directional and multi-scale method to identify gradual transitions. In: XV Brazilian Symposium on Computer Graphics and Image Processing, 2002**, Fortaleza. IEEE SIBGRAPI 2002, 2002. p. 261-268.

---

[Link](http://m-fit.googlecode.com/files/guimarae.pdf)
GUIMARÃES, S. J. F. ; ARAÚJO, Arnaldo de Albuquerque ; COUPRIE, Michel ; LEITE, Neucimar Jerônimo . **Identificação de transições em vídeo digital a partir da análise de ritmo visual,. In: Workshop em Tratamento de Imagens, 2002**, Belo horizonte. Anais (CD-ROM) do III Workshop em Tratamento de Imagens, 2002.

---

[Link](http://m-fit.googlecode.com/files/297-304.pdf)
GUIMARÃES, S. J. F. . **A method for cut detection based on visual rhythm. In: XIV Brazilian Symposium on Computer Graphics and Image Processing, 2001,** Florianópolis. Proc. of the IEEE SIBGRAPI'2001, 2001. p. 297-304.

---

[Link](http://m-fit.googlecode.com/files/ZMM-MM95.pdf) R. Zabih, J. Miller, and K. Mai. **A feature-based algorithms for detecting and classifying
scene breaks.** Multimedia Systems, 7:119–128, 1999.

---

PAGO
_W. A. C. Fernando, C. N. Canagarajah, and D. R. Bull. **Fade and dissolve detection
in uncompressed and compressed video sequences.**In Proc. of the IEEE ICIP, pages
299–303, 1999._

---

_H. Zhang, A. Kanknhalli, and S. W. Smoliar. **Automatic partitioning of full-motion
video. Multimedia Systems, 1:10–28, 1993.**_

---

_M. G. Chung, J. Lee, H. Kim, S. M.-H. Song, and W. M. Kim. **Automatic video segmentation based on spatio-temporal features. Korea Telecom Journal, 4(1):4–14, 1999.**_

---

[Link](http://m-fit.googlecode.com/files/A%20review%20of%20real-time%20segmentation.pdf) Sébastien Lefèvre, Jérôme Holler, Nicole Vincent. **A review of real-time segmentation of uncompressedvid eo sequences for content-basedsearch andretrieval, 2003.**

---

[Link](http://m-fit.googlecode.com/files/A%20new%20cut%20detection%20algorithm.pdf) Tie-Yan Liu, Kwok-Tung Lo,Xu-Dong Zhang,aand Jian Fengc. **A new cut detection algorithm with constant false-alarm ratio for video segmentation, 2002.**

---

[Link](http://m-fit.googlecode.com/files/A%20fuzzy%20logic%20approach%20for%20detection.pdf) Hui Fang, Jianmin Jiang,Yue Feng. **A fuzzy logic approach for detection of video shot boundaries, 2006.**

---

[Link](http://m-fit.googlecode.com/files/An%20efficient%20method%20for%20scene%20cut%20detection.pdf) Jun Yu, M.D. Srinath. **An efficient method for scene cut detection, 2001**

---

[Link](http://m-fit.googlecode.com/files/Circular%20discontinuities%20detection.pdf) Hafizal Yazida, Haniza Yazid, Mohd Haruna, Shukri Mohda, A. Aziz Mohameda,
Mohamed Rizonb, Shaharudin Sayutia. **Circular discontinuities detection in welded joints using Circular Hough Transform, 2007.**

---

[Link](http://m-fit.googlecode.com/files/Content-based%20indexing%20and%20retrieval%20of%20TV%20news.pdf) M. Bertini, A. Del Bimbo, P. Pala. **Content-based indexing and retrieval of TV news, 2001.**

---

[Link](http://m-fit.googlecode.com/files/Indexing%20for%20reuse%20of%20TV%20news%20shots.pdf) M. Bertini, A. Del Bimbo, P. Pala. **Indexing for reuse of TV news shots**, 2000.

---

[Link](http://m-fit.googlecode.com/files/Movie%20scene%20segmentation%20using%20background%20information.pdf) Liang-Hua Chena, Yu-Chun Laib, Hong-Yuan Mark Liao. **Movie scene segmentation using background information**, 2007.

---

[Link](http://m-fit.googlecode.com/files/Object%20based%20video%20retrieval%20with%20local%20region%20tracking.pdf) Arasanathan Anjulan, Nishan Canagarajah. **Object based video retrieval with local region tracking**, 2007.

---

[Link](http://m-fit.googlecode.com/files/Temporal%20video%20segmentation%20and%20classification%20of%20edit%20effects.pdf) Sarah Porter, Majid Mirmehdi, Barry Thomas. **Temporal video segmentation and classification of edit effects**, 2003.

---

[Link](http://m-fit.googlecode.com/files/Journal_00_RITA.pdf)ARAÚJO, Arnaldo de Albuquerque ; GUIMARÃES, S. J. F. . **Recuperação de Informação com Base no Conteúdo Visual em Imagens e Vídeo Digitais.** Revista de Informatica Teórica e Aplicada, Porto Alegre, v. VII, n. 2, p. 43-71, 2000

---

[Link](http://m-fit.googlecode.com/files/Journal_03_prl_jamil.pdf) GUIMARÃES, S. J. F. ; COUPRIE, Michel ; ARAÚJO, Arnaldo de Albuquerque ; LEITE, Neucimar Jerônimo . **Video segmentation based on 2D image analysis.**Pattern Recognition Letters, v. 24, p. 947-957, 2003.

---

[Link](http://www.inf.pucminas.br/projetos/viplab/site/viplab/files/pdf/Journal_04_JASP.pdf) GUIMARÃES, S. J. F. ; LEITE, Neucimar Jerônimo ; COUPRIE, Michel ; ARAÚJO, Arnaldo de Albuquerque . **Flat Zone Analysis and a Sharpening Operation for Gradual Transition Detection on Video Images.** Journal On Applied Signal Processing, Ohio, USA, 2004.

---

Pagina do Guimaraes e TODOS seus artigos. Obs.: Os artigos já estão nesta lista aqui.
