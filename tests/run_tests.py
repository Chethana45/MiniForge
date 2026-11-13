import pathlib, subprocess, sys
root=pathlib.Path(__file__).resolve().parents[1]
exe=pathlib.Path(sys.argv[1]).resolve()
cases=[("hello.mf","Hello, MiniForge\n"),("factorial.mf","120\n"),("arrays.mf","[1, 2, 3, 4]\n4\narray\n"),("loops.mf","0\n1\n2\n3\n4\n"),("closures.mf","1\n2\n")]
bad=0
for name,want in cases:
 p=subprocess.run([str(exe),str(root/"examples"/name)],capture_output=True,text=True)
 if p.returncode or p.stdout!=want:
  bad+=1;print("FAIL",name,"expected",repr(want),"got",repr(p.stdout),p.stderr)
 else: print("PASS",name)
print(f"{len(cases)-bad}/{len(cases)} passed")
sys.exit(bool(bad))
