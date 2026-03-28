import pypdf
import sys

def main():
    pdf_path = sys.argv[1]
    output_path = sys.argv[2]
    reader = pypdf.PdfReader(pdf_path)
    with open(output_path, 'w', encoding='utf-8') as f:
        for i, page in enumerate(reader.pages):
            f.write(f'--- PAGE {i+1} ---\n')
            f.write(page.extract_text() + '\n\n')

if __name__ == "__main__":
    main()
