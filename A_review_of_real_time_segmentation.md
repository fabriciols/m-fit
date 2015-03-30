# RESUMO #

```
Sebastina stevie at all , apresentaram neste paper uma gama de algoritmos referentes
a identificação de transições em videos sem compressão , utilizando métodos baseados em:
pixel, histograma, bloco, caracteristicas, movimento e outros tipos . O foco era analizar
o custo computacional de cada algoritmo à fim de encontrar o melhor . Conclui-se que 
os algoritmos que utilizam logicas simples geralmente tem um custo computacional baixo
porém dispõe de uma qualidade menor nas detecções , em contrapartida , algoritmos que
usam logicas complexas , como analise de movimento e caracteristica , ou um método que 
use varios algoritmos , conseguem ter como resultado final uma boa qualidade em termo
de detecçaõ de transição , porem tem um custo computacional mais alto. O artigo não
tinha como escopo analizar videos com algum tipo de compressão.
```

# TRADUÇÃO #

## Resumo ##

```
Apresentamos neste paper uma revisão de métodos para segmentação de sequências de vídeos 
sem compressão Segmentação de video é normalmente feito no dominio temporal por detecção 
de mudança de omada . No caso de segmentação em tempo-real , complexidade computacional é 
um dos critérios que devem ser levados em conta quando comparando diferentes métodos . 
Quando trabalhando com sequências de video sem compressão, este critério é ainda mais 
significativo. Entretanto , alguns reviews anteriormente publicados não involvem 
critérios de complexidade quando comparam os metodos de detecção de transição . Somente 
taxa de reconhecimento e habilidade de classificar transições detectadas são 
consideradas . Então ao contrário de outros "review" , demonstramos aqui a complexidade 
da maioria dos métodos descritos . Resumimos neste paper um extensiva gama de metodos 
apresentados na literatura e calssificamos eles em varias partes , dependendo da 
informaçãp usada para detectar as transições. Os primeiros métodos comparam frames 
sucessivos confiando no mais simples dos elementos, isto é: o pixel. Comparações podem 
ser feitas a um nivel global , então metodos baseados em histogramas estão sendo 
propostos tambem. Metodos baseados em blocos tem que ser comparados para processar dados 
em um nivel intermediário , entre niveis locais (usando pixels) e  globais (usando 
histogramas). Características mais complexas podem ser envolvidas, resultando em um 
mpetodo baseado em característica. Como alternativa outros métodos confiam no movimento 
como critério para detectar transições. Finalmente , diferentes tipos de informação pode 
ser combinada com o objetivo de aumentar a qualidade da detecção de transição. Então 
nosso "review" irá detalhar métodos de segmentação baseados nas seguintes informações : 
pixel, histograma, bloco , características , movimento , ou outro tipo de informação .
```

## Conclusão ##

```
	Apresentamos neste paper vários métodos para segmentação sequências de video sem
compressão. Focamos na complexidade dos métodos , contrário á reviews anteriormente publicados.
Este critério é particurlamente importande por duas razões principais. Primeiramente ,
videos sem compressão tem uma grande quantia de dados, que não é o caso dos videos comprimidos.
Então o método de "parsear" o video se trtando de videos sem compressão pe caracterizado
pelo seu aspecto de intensidade computacional . Por segundo , temos que usar metodos conhecidos
pelo seu baixo custo computacional para processar um video pela sua taxa de frames .
Resultados deste estudo demonstram que o melhor metodo para segmentação em tempo-real de 
videos sem compressão devem ser selecionados considerando eficiência e custo computacional. 
Métodos simples como diferença entre frames é um dos metodos mais rápidos , porém são
caracterizados pela baixa qualidade. Por outro lado, métodos baseados em cracteristica/movimento
ou metodos combinando vários algoritmos são mais robustos e levam para uma melhor qualidade,
mas eles são conhecidos por utilizar muitos recursos computacionais. Outro critério importante
para escolher um método é como escolher limites computacionais e o numero de limites .
Trabalhos futuros trabalharam em cima do estudo deste critério.
```