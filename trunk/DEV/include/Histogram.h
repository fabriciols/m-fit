/**
 *Classe respons�vel por conter
 *os dados do histograma de um frame.
 */
class Histogram 
{
 
	/**
	 *Armazena os dados do histograma
	 *da imagem.
	 */
	private:
		int data[255];
	 
	/**
	 *Guarda o maior valor
	 *de lumin�ncia do histograma
	 */
		int max;
	 
	/**
	 *Armazena o menor valor
	 *de lumin�ncia do histograma
	 */
		int min;
};
