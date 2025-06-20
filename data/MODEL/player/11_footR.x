xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 32;
 10.81113;-20.22292;-13.10284;,
 -10.24609;-20.22292;-13.10284;,
 -10.24609;-20.22292;-35.53439;,
 10.81113;-20.22292;-35.53439;,
 10.80972;-9.70673;-13.10284;,
 5.54628;-4.44419;-13.10284;,
 -4.98130;-4.44419;-13.10284;,
 -10.24468;-9.70673;-13.10284;,
 10.80972;-9.70673;-35.53439;,
 5.54628;-4.44419;-30.27170;,
 -10.24468;-9.70673;-35.53439;,
 -4.98130;-4.44419;-30.27170;,
 10.81113;-20.22292;-35.53439;,
 -10.24609;-20.22292;-35.53439;,
 -10.24609;-20.22292;-13.10284;,
 -10.24468;-9.70673;-13.10284;,
 10.81113;-20.22292;-13.10284;,
 10.80972;-9.70673;-13.10284;,
 13.61346;-20.28867;-13.54202;,
 -13.04841;-20.28867;-13.54202;,
 -13.04579;-0.30990;-13.54202;,
 13.61078;-0.30990;-13.54202;,
 -13.04579;-0.30990;14.86000;,
 13.61078;-0.30990;14.86000;,
 -13.04841;-20.28867;14.86000;,
 13.61346;-20.28867;14.86000;,
 -13.04841;-20.28867;-13.54202;,
 13.61346;-20.28867;-13.54202;,
 -13.04841;-20.28867;14.86000;,
 -13.04579;-0.30990;14.86000;,
 13.61346;-20.28867;14.86000;,
 13.61078;-0.30990;14.86000;;
 
 18;
 4;0,1,2,3;,
 3;0,4,1;,
 3;4,5,1;,
 3;5,6,1;,
 3;6,7,1;,
 4;8,9,5,4;,
 4;9,8,10,11;,
 4;11,10,7,6;,
 4;12,13,10,8;,
 4;9,11,6,5;,
 4;13,14,15,10;,
 4;16,12,8,17;,
 4;18,19,20,21;,
 4;21,20,22,23;,
 4;23,22,24,25;,
 4;25,24,26,27;,
 4;19,28,29,20;,
 4;30,18,21,31;;
 
 MeshMaterialList {
  11;
  18;
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8;;
  Material {
   0.800000;0.568000;0.408000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.032000;0.024000;0.024000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.568000;0.408000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.624000;0.624000;0.624000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.624000;0.624000;0.624000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.624000;0.624000;0.624000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.496000;0.360000;0.216000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.056000;0.056000;0.056000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.112000;0.064000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.112000;0.120000;0.112000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.200000;0.112000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  17;
  0.000000;-1.000000;0.000000;,
  0.923838;0.382785;0.000000;,
  0.382644;0.923896;0.000000;,
  0.000000;0.923882;-0.382677;,
  0.000000;0.382690;-0.923877;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.707117;-0.707096;,
  -0.707050;0.707164;0.000000;,
  -0.923838;0.382783;0.000000;,
  -0.382646;0.923895;0.000000;,
  -1.000000;0.000134;0.000000;,
  1.000000;0.000134;0.000000;,
  -1.000000;0.000131;0.000000;,
  1.000000;0.000134;0.000000;;
  18;
  4;0,0,0,0;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,5,5;,
  3;5,6,5;,
  4;1,2,2,1;,
  4;9,4,4,3;,
  4;10,11,11,12;,
  4;7,7,4,4;,
  4;2,3,12,2;,
  4;13,13,11,11;,
  4;14,14,1,1;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;15,15,15,15;,
  4;16,16,16,16;;
 }
 MeshTextureCoords {
  32;
  0.375000;0.750000;,
  0.625000;0.750000;,
  0.625000;1.000000;,
  0.375000;1.000000;,
  0.375000;0.583380;,
  0.437490;0.500000;,
  0.562510;0.500000;,
  0.625000;0.583380;,
  0.375000;0.166620;,
  0.437490;0.308650;,
  0.625000;0.166620;,
  0.562510;0.308650;,
  0.375000;0.000000;,
  0.625000;0.000000;,
  0.875000;0.000000;,
  0.875000;0.166620;,
  0.125000;0.000000;,
  0.125000;0.166620;,
  0.375000;0.000000;,
  0.625000;0.000000;,
  0.625000;0.250000;,
  0.375000;0.250000;,
  0.625000;0.500000;,
  0.375000;0.500000;,
  0.625000;0.750000;,
  0.375000;0.750000;,
  0.625000;1.000000;,
  0.375000;1.000000;,
  0.875000;0.000000;,
  0.875000;0.250000;,
  0.125000;0.000000;,
  0.125000;0.250000;;
 }
}
