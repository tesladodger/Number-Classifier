int[][] grid;

BufferedReader br;
double[][] W1;
double[][] W2;
double[] B1;
double[] B2;
double[] inputs;
double[] hiddens;
double[] outputs;


void setup () {
  size(640, 700); //640);
  grid = new int[32][32];

  W1 = new double[16][14];
  W2 = new double[14][10];
  B1 = new double[14];
  B2 = new double[10];
  hiddens = new double[14];
  outputs = new double[10];

  readFile();

  textSize(30);
}


void draw () {
  background(0);
  fill(230);
  noStroke();

  rect(0, 640, 640, 60);

  fill(255);
  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 32; j++) {
      if (grid[i][j] == 1) {
        rect(i * 20, j * 20, 20, 20);
      }
    }
  }

  // TODO:
  //   - predict
  //   - correct ??
  fill((mouseX >= 5 && mouseX <= 155 && mouseY >= 645 && mouseY <= 695) ? 100 : 80);
  rect(5, 645, 150, 50);
  fill((mouseX >= 160 && mouseX <= 310 && mouseY >= 645 && mouseY <= 695) ? 100 : 80);
  rect(160, 645, 150, 50);


  fill(255);
  text("Reset", 35, 680);
  text("Predict", 185, 680);
}


void mouseDragged () {
  if (mouseX < 0 || mouseX > 639 || mouseY < 0 || mouseY > 639) return;
  grid[mouseX/20][mouseY/20] = 1;
  if (mouseX > 19) grid[mouseX/20-1][mouseY/20] = 1;
  if (mouseX < 619) grid[mouseX/20+1][mouseY/20] = 1;
  if (mouseY > 19) grid[mouseX/20][mouseY/20-1] = 1;
  if (mouseY < 619) grid[mouseX/20][mouseY/20+1] = 1;
}


void mouseClicked () {
  if (mouseX >= 5 && mouseX <= 155 && mouseY >= 645 && mouseY <= 695) {
    for (int i = 0; i < 32; i++) {
      for (int j = 0; j < 32; j++) {
        grid[i][j] = 0;
      }
    }
    return;
  }

  if (mouseX >= 160 && mouseX <= 310 && mouseY >= 645 && mouseY <= 695) {
    feedForward();
    return;
  }

  if (mouseX >= 0 && mouseX < 640 && mouseY >= 0 && mouseY < 640) {
    grid[mouseX/20][mouseY/20] = 1;
  } else {
    return;
  }
  if (mouseX > 19) grid[mouseX/20-1][mouseY/20] = 1;
  if (mouseX < 619) grid[mouseX/20+1][mouseY/20] = 1;
  if (mouseY > 19) grid[mouseX/20][mouseY/20-1] = 1;
  if (mouseY < 619) grid[mouseX/20][mouseY/20+1] = 1;
}


void feedForward () {
  inputs = new double[16];
  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 32; j++) {
      if (grid[i][j] == 1) {
        int col = i >> 3;
        int row = j >> 3;
        inputs[4*row+col] += 1;
      }
    }
  }
  
  
  for (int i = 0; i < 14; i++) {
    hiddens[i] = 0;
    for (int j = 0; j < 16; j++) {
      hiddens[i] += inputs[j] * W1[j][i];
    }
    hiddens[i] += B1[i];
    hiddens[i] = sigmoid(hiddens[i]);
  }
  
  for (int i = 0; i < 10; i++) {
    outputs[i] = 0;
    for (int j = 0; j < 14; j++) {
      outputs[i] += hiddens[j] * W2[j][i];
    }
    outputs[i] += B2[i];
    outputs[i] = sigmoid(outputs[i]);
  }
  
  for (double out : outputs) System.out.println(out);
  int prediction = 0;
  for (int i = 0; i < 10; i++) {
    if (outputs[i] > outputs[prediction]) {
      prediction = i;
    }
  }
  System.out.println(prediction);
  
}


double sigmoid (double x) {
  return 1 / ( 1 + Math.pow(Math.E, -4.9*x ));
}


void readFile () {
  br = createReader("nn.csv");
  String line;
  int lCounter = 0;
  do {
    try {
      line = br.readLine();
    } 
    catch (IOException e) {
      System.out.println(e.getMessage());
      line = null;
    }

    String[] vals = split(line, ",");

    if (lCounter < 16) {
      for (int i = 0; i < 14; i++) {
        W1[lCounter][i] = Double.parseDouble(vals[i]);
      }
    } else if (lCounter < 30) {
      for (int i = 0; i < 10; i++) {
        W2[lCounter-16][i] = Double.parseDouble(vals[i]);
      }
    } else if (lCounter == 30) {
      for (int i = 0; i < 14; i++) {
        B1[i] = Double.parseDouble(vals[i]);
      }
    } else {
      for (int i = 0; i < 10; i++) {
        B2[i] = Double.parseDouble(vals[i]);
      }
    }
  } while (++lCounter < 32);
}
