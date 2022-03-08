import os
current_dir=os.path.dirname(os.path.abspath(__file__))

def main():
    image_path=os.path.join(current_dir,'dataset/images')
    images=os.listdir(image_path)
    print(f'images: {len(images)}')

    labels_path=os.path.join(current_dir,'dataset/labels')
    labels=os.listdir(labels_path)
    print(f'labels: {len(labels)}')

    assert len(labels)==len(images)

    csv_lines=[]

    for label in labels:
        label_path=os.path.join(current_dir,'dataset/labels',label)
        with open(label_path) as f:
            image_name=label.replace('.txt','',1)
            img_path=f'dataset/images/{image_name}.jpg'  
            lines=f.readlines()
            for line in lines:
                # line example: 1 0.538086 0.871353 0.089844 0.111406
                infos=line.strip().split(' ')
                if len(infos)==5:
                    final_info=f'UNASSIGNED,{img_path},cls_{infos[0]},{infos[1]},{infos[2]},,,{infos[3]},{infos[4]},,\n'
                    # print(final_info)
                    csv_lines.append(final_info)
    
    # print(csv_lines)
    with open(os.path.join(current_dir,'labels.csv'),'w+') as f:
        f.writelines(csv_lines)
    pass

if __name__ =='__main__':
    main()