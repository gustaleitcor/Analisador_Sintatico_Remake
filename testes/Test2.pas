program Test2;
var
   X, A, B: integer;
begin
   A := 5 + (7 + 8);
   B := 10;
   if (A > B) then
   begin
      X := (A);
      A := B;
      B := X;   
   end;
   else 
   begin
   end;
   
    {alguns alunos preferem implementar com ;}
    {veja que o �ltimo comando de um bloco n�o possui o ;}

end.	       

{gere erros sint�ticos}