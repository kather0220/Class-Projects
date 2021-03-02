package FinalExamProject;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;

public class DateSet extends JFrame{
	
	JComboBox<String> yearBox = new JComboBox<String>();{	// 년도를 선택하는 JComboBox에 반복문을 이용하여 년도를 추가
		for(int i=1;i<=19;i++) {
			if(i<=9)
				yearBox.addItem("200"+Integer.toString(i));
			else
				yearBox.addItem("20"+Integer.toString(i));
		}
	}
	
	JComboBox<String> monthBox = new JComboBox<String>();{	// 월를 선택하는 JComboBox에 반복문을 이용하여 월을 추가
		for(int i=1;i<=12;i++) {
			if(i<=9)
				monthBox.addItem("0"+Integer.toString(i));
			else
				monthBox.addItem(Integer.toString(i));
		}
	}
	JComboBox<String> dateBox = new JComboBox<String>();{	// 일을 선택하는 JComboBox에 반복문을 이용하여 일을 추가
	for(int i=1; i<=31; i++) {
		
		if(i<=9)
			dateBox.addItem("0"+Integer.toString(i));
		else
			dateBox.addItem(Integer.toString(i));
		}	
	}
	
	
	

	String selectedYear="2001";		// 년도와 월, 일을 저장하는 변수를 선언하고 초기값을 지정해줌
	String selectedMonth="01";
	String selectedDate="01";
	
	
	public DateSet() throws IOException, InterruptedException {
		setTitle("과거의 날씨 데이터");
		
		final JPanel dateSelectMenu= new JPanel();	// dataSelectMenu : 처음 화면 ( final로 선언한 것은 다른 이벤트 리스너에서 접근할 수 있도록 하기 위함 )
		dateSelectMenu.setLayout(new BorderLayout());	
		dateSelectMenu.setBackground(Color.getHSBColor(20, 20, 20));	// 수치를 직접 입력하여 적절한 배경색을 지정함
		
		Font f = new Font("",Font.PLAIN,20);	// 폰트 객체
		
		
		JLabel welcomeText = new JLabel("날씨 데이터 알리미입니다.");
		JLabel chooseDate = new JLabel("날짜를 선택하세요!");
		JButton btn1 = new JButton("선택 완료");
		
		welcomeText.setFont(f);
		chooseDate.setFont(f);
		
		btn1.setBackground(Color.white);	// 버튼과 콤보 박스 색깔을 지정해줌
		yearBox.setBackground(Color.white);
		monthBox.setBackground(Color.white);
		dateBox.setBackground(Color.white);
		
		
		yearBox.setBounds(160, 100, 80, 40);	// 각 컴퍼넌트의 위치를 지정해줌
		monthBox.setBounds(260, 100, 80, 40);
		dateBox.setBounds(360,100,80,40);
		welcomeText.setBounds(190, 10, 300, 20);
		chooseDate.setBounds(220, 40, 300, 20);
		btn1.setBounds(260, 300, 100, 40);
		
		dateSelectMenu.setLayout(null);	// 컴퍼넌트의 위치를 자유롭게 추가하기 위해 기존 레이아웃을 삭제함
		dateSelectMenu.add(yearBox);
		dateSelectMenu.add(monthBox);
		dateSelectMenu.add(dateBox);
		dateSelectMenu.add(chooseDate);
		dateSelectMenu.add(welcomeText);
		dateSelectMenu.add(btn1);
		add(dateSelectMenu);	// 패널을 추가함
		

		
		yearBox.addActionListener(new ActionListener() {	// 년도를 선택하는 콤보박스에 이벤트 리스너를 넣어줌

			@Override
			public void actionPerformed(ActionEvent e) {
				JComboBox<String> yearInfo = (JComboBox<String>)e.getSource();
				selectedYear = yearInfo.getSelectedItem().toString();	// 사용자가 선택한 년도 데이터를 가져와서 변수에 저장해줌
				
			}
			
		});
		
		monthBox.addActionListener(new ActionListener() {	// 월을 선택하는 콤보박스에 이벤트 리스너를 넣어줌

			@Override
			public void actionPerformed(ActionEvent e) {
				JComboBox<String> monthInfo = (JComboBox<String>)e.getSource();
				selectedMonth = monthInfo.getSelectedItem().toString();		// 사용자가 선택한 월 데이터를 가져와서 변수에 저장해줌
				
			}
			
		});
		
		dateBox.addActionListener(new ActionListener() {	// 일을 선택하는 콤보박스에 이벤트 리스너를 넣어줌

			@Override
			public void actionPerformed(ActionEvent e) {
				JComboBox<String> dateInfo = (JComboBox<String>)e.getSource();
				selectedDate = dateInfo.getSelectedItem().toString();	// 사용자가 선택한 일 데이터를 가져와서 변수에 저장해줌

				
			}
			
		});
		
		class MyGUI implements ActionListener{		// 선택 완료 버튼에 넣을 이벤트 리스너를 내부 클래스로 만들어 줌
			
			@Override
			public void actionPerformed(ActionEvent e) {
				
				FileReader fr = null;	// 파일을 읽어오기 위한 객체 선언
				try {
					fr = new FileReader("weather_data.csv");
				} catch (FileNotFoundException e1) {	// 파일이 존재하지 않았을 때의 예외 처리
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				
				String line = null;
				
				String average, lowest, highest;
				average = lowest = highest = null;
				
				String searchDate = (selectedYear+"-"+selectedMonth+"-"+selectedDate);	
				// searchDate 변수를 이런 형식으로 만든 것은 읽어온 파일의 데이터와 직접적인 비교를 하기 위한 것임
				
				System.out.println(searchDate);
				
				BufferedReader br = new BufferedReader(fr);		// 파일을 한 줄씩 읽기 위한 객체
				int count=0;
				try {
					while((line = br.readLine())!=null) {	// 파일을 끝까지 읽음
						String arr[] = line.split(",");
						for(String s:arr) {	// 데이터를 순차적으로 모두 접근함
							if(searchDate.equals(arr[0])) {		// 사용자가 입력한 날짜와 데이터의 날짜가 일치할 때
								
								if(count ==1) {	
									average = s;
									System.out.println("평균: "+average);
								}
								if(count == 2) {
									lowest = s;
									System.out.println("최저 : "+lowest);
								}
								if(count == 3) {
									highest = s;
									System.out.println("최고: "+highest);
								}
								
								 count++;
								 // count를 저장하는 이유는 순환문을 돌 때마다 평균, 최저, 최고 기온 데이터를 순차적으로 가져오기 위한 것임
								
								}
					
							}
						
						}
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				
				try {
					br.close();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				
				
				dateSelectMenu.setVisible(false);	// 처음 화면을 안보이게 만듦
				
				final JPanel noticeWeatherMenu = new JPanel();	// 선택 완료 버튼을 눌렀을 때 나타날 새 패널을 선언
				noticeWeatherMenu.setBackground(Color.PINK);
				add(noticeWeatherMenu);
				noticeWeatherMenu.setVisible(true);
				
				double averageInNum = 0;	// 평균 기온 데이터를 실수형으로 변환하여 저장하기 위해 변수 선언
				
				try {	// 만약 사용자가 입력한 날짜가 존재하지 않은 날짜였을 경우, average에는 데이터도 저장되지 않기 때문에 에러가 발생함
				averageInNum = Double.parseDouble(average);
				
				Font f2 = new Font("",Font.BOLD,14);
				// 평균, 최저, 최고 기온을 화면에 출력함
				JLabel text1 = new JLabel(searchDate+"의 평균 기온은  "+average+"도 이고 최저 기온은 "+lowest+"도 이며 최고 기온은 "+highest+"도 입니다.");
				text1.setFont(f2);
				noticeWeatherMenu.add(text1,BorderLayout.NORTH);
				
				JLabel text2;
				JLabel image;
				// 그림을 가져옴
				ImageIcon img1 = new ImageIcon("weather-snowman.jpg");
				ImageIcon img2 = new ImageIcon("weather-wind.jpg");
				ImageIcon img3 = new ImageIcon("weather-breeze.jpg");
				ImageIcon img4 = new ImageIcon("weather-sunny.jpg");
				
				// 평균 기온의 범위를 나누어 범위에 따라 다른 날씨 문구와 그림이 화면에 표시 됨
				if(averageInNum<=0) {
					
					image = new JLabel(img1);
					noticeWeatherMenu.add(image,BorderLayout.CENTER);
					text2 = new JLabel("날씨가 매우 추우니 옷을 든든하게 입으시는게 좋겠습니다.");
					text2.setFont(f2);
					noticeWeatherMenu.add(text2,BorderLayout.SOUTH);
					
				}
				
				if(averageInNum>0&&averageInNum<=10) {
					
					image = new JLabel(img2);
					noticeWeatherMenu.add(image,BorderLayout.CENTER);
					text2 = new JLabel("날씨가 쌀쌀하니 감기 조심하시길 바랍니다.");
					text2.setFont(f2);
					noticeWeatherMenu.add(text2,BorderLayout.SOUTH);
					
				}
				
				if(averageInNum>10&&averageInNum<=20) {
					
					image = new JLabel(img3);
					noticeWeatherMenu.add(image,BorderLayout.CENTER);
					text2 = new JLabel("날씨가 선선한 하루가 되겠습니다.");
					text2.setFont(f2);
					noticeWeatherMenu.add(text2,BorderLayout.SOUTH);
					
				}
				
				if(averageInNum>20) {
					
					image = new JLabel(img4);
					noticeWeatherMenu.add(image,BorderLayout.CENTER);
					text2 = new JLabel("날씨가 다소 덥겠습니다. 옷을 가볍게 입으시는게 좋겠습니다.");
					text2.setFont(f2);
					noticeWeatherMenu.add(text2,BorderLayout.SOUTH);
					
				}
				
			}
				
				catch(NullPointerException e1) {	
					// 사용자가 존재하지 않은 날짜를 입력했을 때, 선택 완료 버튼 누룬 후 나타나는 화면을 보이도록 하지 않고, 기존으 화면에 다시 입력하라는 메시지를 출력함
					noticeWeatherMenu.setVisible(false);
					dateSelectMenu.setVisible(true);
					JLabel errorMessage = new JLabel("존재하지 않는 날짜입니다. 다시 입력해주세요.");
					errorMessage.setBounds(190, 200, 400, 20);
					dateSelectMenu.add(errorMessage);
				}
				
				
			}
		}
		
		btn1.addActionListener(new MyGUI());	// 선택완료 버튼에 이벤트 리스너를 추가함
			
		setSize(600,500);
		setVisible(true);	
	}
	
	
	public static void main(String[] args) throws IOException, InterruptedException{
		
		new DateSet();
				
		
		}
		
	}
	
	


