
void displayTitle()
{
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);    
  u8x8.clear();
  u8x8.inverse();
  uint8_t r;
  for( r = 0; r < u8x8.getCols(); r++ )
  {
    u8x8.setCursor(r, 0);
    u8x8.print(" ");
  }
  u8x8.setCursor(1, 0);
  u8x8.print("MegatecSIM");
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.noInverse();
//  u8x8.setCursor(0,1);

}

void displayRenderCurrentPage()
{
  u8x8.setCursor(0,1);
  u8x8.print("Temp: ");
  u8x8.println(temperature, 1);
}

void notImplemented()
{
  u8x8.println("\nNot\nImplemented");
}
