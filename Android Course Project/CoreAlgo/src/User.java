

public class User {
	final String zero="00000000";
	final int MAX=15000;
	String xm;
	int xh;
	Course course[][];
	
	User(){
		
	}
	
	User(int id){
		xh=id;
		course=new Course[100][200];
		for(int i=0;i<13;i++){
			//course[i]=new Course[20];
			for(int j=0;j<5;j++){
				course[i][j]=new Course();
				course[i][j].vis=false;
				course[i][j].kh=zero;
			}
		}
	}
	
	//�Ƿ��иÿγ�
	public boolean existcourse(String kh){
		for(int i = 0; i < MAX ; i++){
		//	if() return true;
		}
		return false;
	}
		
		
		
	public boolean selectcourse(String kh){
		//�������Ŀκ��������ݿ��в����ڴ˿�
//		if() return false;
//		���ѡ������δ������ʱ���ͻ��day�ܼ���st��ʼ�Ͽ�ʱ�䣬ed�����Ͽ�ʱ��
		int st=0,ed=0,day=0;
//		for(int i = 0 ; i < MAX ; i++ ){
			//����ҵ��ÿκ�,�ҳ��Ͽ�ʱ��
//			if(){
//				st=...;
//				ed=...;
//				day=...;		
//				for(int time = st; time <= ed; time++){
//					 if(course[time][day].vis==true){
//						 return false;
//					 }
//				 }
//			}
//		}		
		//��Ǹ�ʱ��ѡ�ÿγ�
//		for(int time = st; time <= ed; time++){
//			
//		}		
		return true;
	}
	
	
	//ɾ����ѡ�γ�
	public boolean removecourse(String kh){
		return true;
	}
	
	public void showcourse(){
		for(int i=0;i<13;i++){
			for(int j=0;j<5;j++){
				System.out.printf("%s ",course[i][j].kh);
			}
			System.out.println();
		}
	}
	
}
