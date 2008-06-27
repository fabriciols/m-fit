/**
 *Classe responsável por conter
 *os dados do histograma de um frame.
 */
class Histogram 
{
 
	/**
	 *Armazena os dados do histograma
	 *da imagem.
	 */
	public:
		Histogram(IplImage* img);
		double* getMatrix();
		float getMax();
		float getMin();
		int getMaxIdx();
		int getMinIdx();


	private:


		double data[255];
	 
	/**
	 *Guarda o maior valor
	 *de luminância do histograma
	 */
		float max;
		int idx_max;
	 
	/**
	 *Armazena o menor valor
	 *de luminância do histograma
	 */
		float min;
		int idx_min;
};
