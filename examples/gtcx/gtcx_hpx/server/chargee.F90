subroutine chargee(hpx4_bti,&
             t_gids, p_gids,&
! global parameters
                       ihistory,snapout,maxmpsi,&
          mi,mimax,me,me1,memax,mgrid,mpsi,mthetamax,mzeta,mzetamax,&
       istep,ndiag,ntracer,msnap,mstep,mstepall,stdout,mype,numberpe,&
       mode00,nbound,irun,iload,irk,idiag,ncycle,mtdiag,idiag1,idiag2,&
       ntracer1,nhybrid,ihybrid,nparam,rng_control,limit_vpara,fixed_Tprofile,&
           nonlinear,paranl,a0,a1,a,q0,q1,q2,pi,tstep,kappati,kappate,kappan,&
       flow0,flow1,flow2,ulength,utime,gyroradius,deltar,deltaz,zetamax,&
       zetamin,umax,tite,rc,rw,tauii,qion,qelectron,aion,aelectron,&
                                       mtheta, &
                                       deltat, &
           do_collision, &
! particle array
                                      kzion,kzelectron,jtelectron0,jtelectron1,&
                                        jtion0,jtion1,&
                                       wzion,wzelectron,wpelectron,&
       wtelectron0,wtelectron1, &
                                        wpion,wtion0,wtion1,&
                                         zion,zion0,zelectron,&
        zelectron0,zelectron1, &
! field array
                      mmpsi, &
                                      itran,igrid, &
                                          jtp1,jtp2,&
                                       phi00,phip00,rtemi,rteme,rden,qtinv,&
       pmarki,pmarke,zonali,zonale,gradt,&
                                         phi,densityi,densitye,markeri,&
       markere,pgyro,tgyro,dtemper,heatflux,phit,&
                                           evector,wtp1,wtp2,phisave,&
              Total_field_energy, &
! diagnosis array
                       mflux,num_mode,m_poloidal,&
          nmode,mmode,&
           efluxi,efluxe,pfluxi,pfluxe,ddeni,ddene,dflowi,dflowe,&
       entropyi,entropye,efield,eradial,particles_energy,eflux,&
       rmarker,rdtemi,rdteme,pfluxpsi,&
       amp_mode, &
                                       hfluxpsi,&
                                           eigenmode,&
           etracer,ptracer, &
! particle decomp
              ntoroidal,npartdom,&
              partd_comm,nproc_partd,myrank_partd,&
              toroidal_comm,nproc_toroidal,myrank_toroidal,&
              left_pe,right_pe,&
              toroidal_domain_location,particle_domain_location)

  !use global_parameters
  !use particle_array
  !use field_array
  !use diagnosis_array
  !use particle_decomp
  use, intrinsic :: iso_c_binding, only : c_ptr
  use precision
  implicit none

  TYPE(C_PTR), INTENT(IN), VALUE :: hpx4_bti
  integer,dimension(:),allocatable :: t_gids
  integer,dimension(:),allocatable :: p_gids

!  global parameters
  integer :: ihistory,snapout,maxmpsi
  integer mi,mimax,me,me1,memax,mgrid,mpsi,mthetamax,mzeta,mzetamax,&
       istep,ndiag,ntracer,msnap,mstep,mstepall,stdout,mype,numberpe,&
       mode00,nbound,irun,iload,irk,idiag,ncycle,mtdiag,idiag1,idiag2,&
       ntracer1,nhybrid,ihybrid,nparam,rng_control,limit_vpara,fixed_Tprofile
  real(kind=wp) nonlinear,paranl,a0,a1,a,q0,q1,q2,pi,tstep,kappati,kappate,kappan,&
       flow0,flow1,flow2,ulength,utime,gyroradius,deltar,deltaz,zetamax,&
       zetamin,umax,tite,rc,rw,tauii,qion,qelectron,aion,aelectron
  integer,dimension(:),allocatable :: mtheta
  real(kind=wp),dimension(:),allocatable :: deltat
  logical  do_collision

! particle array
  integer,dimension(:),allocatable :: kzion,kzelectron,jtelectron0,jtelectron1
  integer,dimension(:,:),allocatable :: jtion0,jtion1
  real(kind=wp),dimension(:),allocatable :: wzion,wzelectron,wpelectron,&
       wtelectron0,wtelectron1
  real(kind=wp),dimension(:,:),allocatable :: wpion,wtion0,wtion1
  real(kind=wp),dimension(:,:),allocatable :: zion,zion0,zelectron,&
        zelectron0,zelectron1

! field array
  integer :: mmpsi
  integer,dimension(:),allocatable :: itran,igrid
  integer,dimension(:,:,:),allocatable :: jtp1,jtp2
  real(kind=wp),dimension(:),allocatable :: phi00,phip00,rtemi,rteme,rden,qtinv,&
       pmarki,pmarke,zonali,zonale,gradt
  real(kind=wp),dimension(:,:),allocatable :: phi,densityi,densitye,markeri,&
       markere,pgyro,tgyro,dtemper,heatflux,phit
  real(kind=wp),dimension(:,:,:),allocatable :: evector,wtp1,wtp2,phisave
  real(kind=wp) :: Total_field_energy(3)

! diagnosis array
  integer :: mflux,num_mode,m_poloidal
  integer nmode(num_mode),mmode(num_mode)
  real(kind=wp) efluxi,efluxe,pfluxi,pfluxe,ddeni,ddene,dflowi,dflowe,&
       entropyi,entropye,efield,eradial,particles_energy(2),eflux(mflux),&
       rmarker(mflux),rdtemi(mflux),rdteme(mflux),pfluxpsi(mflux),&
       amp_mode(2,num_mode,2)
  real(kind=wp),dimension(:),allocatable :: hfluxpsi
  real(kind=wp),dimension(:,:,:),allocatable :: eigenmode
  real(kind=wp) etracer,ptracer(4)

! particle decomp
  integer  :: ntoroidal,npartdom
  integer  :: partd_comm,nproc_partd,myrank_partd
  integer  :: toroidal_comm,nproc_toroidal,myrank_toroidal
  integer  :: left_pe,right_pe
  integer  :: toroidal_domain_location,particle_domain_location

  integer m,i,im,j,k,ip,jt,kk,ii,j11,j10,j01,j00,ierror,larmor,ij,ipjt,&
       icount,idest,isource,isendtag,irecvtag
  real(kind=wp) weight,wemark,rdum,tdum,delr,delt(0:mpsi),delz,r,wz1,wz0,wp1,&
       wp0,wt11,wt10,wt01,wt00,adum(0:mpsi),tflr,damping,pi2_inv,&
       psitmp,thetatmp,zetatmp,deltheta
  real(kind=wp) sendl(mgrid),recvr(mgrid)
  real(kind=wp) dnitmp(0:mzeta,mgrid)

  delr=1.0/deltar 
  delt=2.0*pi/deltat
  delz=1.0/deltaz
  pi2_inv=0.5/pi
  densitye=0.0

  do m=1,me
     psitmp=zelectron(1,m)
     thetatmp=zelectron(2,m)
     zetatmp=zelectron(3,m)

     r=sqrt(2.0*psitmp)
     ip=max(0,min(mpsi,int((r-a0)*delr+0.5)))
     jt=max(0,min(mtheta(ip),int(thetatmp*pi2_inv*delt(ip)+0.5)))
     ipjt=igrid(ip)+jt
     wz1=(zetatmp-zetamin)*delz
     kk=max(0,min(mzeta-1,int(wz1)))
     kzelectron(m)=kk
     wzelectron(m)=wz1-real(kk)

!     rdum=delr*max(0.0d+00,min(a1-a0,r-a0))
     rdum=delr*max(ZERO,min(a1-a0,r-a0))
     ii=max(0,min(mpsi-1,int(rdum)))
     wp1=rdum-real(ii)
     wpelectron(m)=wp1

! particle position in theta
     tflr=thetatmp

! inner flux surface
     im=ii
     tdum=pi2_inv*(tflr-zetatmp*qtinv(im))+10.0
     tdum=(tdum-aint(tdum))*delt(im)
     j00=max(0,min(mtheta(im)-1,int(tdum)))
     jtelectron0(m)=igrid(im)+j00
     wtelectron0(m)=tdum-real(j00)

! outer flux surface
     im=ii+1
     tdum=pi2_inv*(tflr-zetatmp*qtinv(im))+10.0
     tdum=(tdum-aint(tdum))*delt(im)
     j01=max(0,min(mtheta(im)-1,int(tdum)))
     jtelectron1(m)=igrid(im)+j01
     wtelectron1(m)=tdum-real(j01)

  enddo	

  do m=1,me
     weight=zelectron(5,m)

     kk=kzelectron(m)
     wz1=weight*wzelectron(m)
     wz0=weight-wz1     

     wp1=wpelectron(m)
     wp0=1.0-wp1

     wt10=wp0*wtelectron0(m)
     wt00=wp0-wt10

     wt11=wp1*wtelectron1(m)
     wt01=wp1-wt11

! If no loop-level parallelism, use original algorithm (write directly
! into array "densitye()".
     ij=jtelectron0(m)
     densitye(kk,ij) = densitye(kk,ij) + wz0*wt00
     densitye(kk+1,ij)   = densitye(kk+1,ij)   + wz1*wt00

     ij=ij+1
     densitye(kk,ij) = densitye(kk,ij) + wz0*wt10
     densitye(kk+1,ij)   = densitye(kk+1,ij)   + wz1*wt10

     ij=jtelectron1(m)
     densitye(kk,ij) = densitye(kk,ij) + wz0*wt01
     densitye(kk+1,ij)   = densitye(kk+1,ij)   + wz1*wt01

     ij=ij+1
     densitye(kk,ij) = densitye(kk,ij) + wz0*wt11
     densitye(kk+1,ij)   = densitye(kk+1,ij)   + wz1*wt11

  enddo

! If we have a particle decomposition on the toroidal domains, do a reduce
! operation to add up all the contributions to charge density on the grid
  if(npartdom>1)then
    do ij=1,mgrid
       do kk=0,mzeta
          dnitmp(kk,ij)=densitye(kk,ij)
          densitye(kk,ij)=0.
       enddo
    enddo
    !call MPI_ALLREDUCE(dnitmp,densitye,(mgrid*(mzeta+1)),mpi_Rsize,&
    !                   MPI_SUM,partd_comm,ierror)
    call partd_allreduce_cmm(hpx4_bti,dnitmp,densityi,mgrid,mzeta+1)
  endif


! poloidal end cell
  do i=0,mpsi
     densitye(:,igrid(i)+mtheta(i))=densitye(:,igrid(i)+mtheta(i))+densitye(:,igrid(i))
  enddo

! toroidal end cell
  sendl=densitye(0,:)
  recvr=0.0
  icount=mgrid
  !!idest=mod(mype-1+numberpe,numberpe)
  idest=left_pe
  !!isource=mod(mype+1,numberpe)
  isource=right_pe
  !!isendtag=mype
  isendtag=myrank_toroidal
  irecvtag=isource

! send densitye to left and receive from right
  !call MPI_SENDRECV(sendl,icount,mpi_Rsize,idest,isendtag,&
  !     recvr,icount,mpi_Rsize,isource,irecvtag,toroidal_comm,istatus,ierror)
  call sndrecv_toroidal_cmm(hpx4_bti,sendl,icount, &
                            recvr,icount,idest)
     
  if(myrank_toroidal == ntoroidal-1)then
! B.C. at zeta=2*pi is shifted
     do i=0,mpsi
        ii=igrid(i)
        jt=mtheta(i)
        densitye(mzeta,ii+1:ii+jt)=densitye(mzeta,ii+1:ii+jt)+&
             cshift(recvr(ii+1:ii+jt),itran(i))
     enddo
  else
! B.C. at zeta<2*pi is continuous
     densitye(mzeta,:)=densitye(mzeta,:)+recvr
  endif
  
! zero out charge in radial boundary cell
  do i=0,nbound-1
     densitye(:,igrid(i):igrid(i)+mtheta(i))=&
        densitye(:,igrid(i):igrid(i)+mtheta(i))*real(i)/real(nbound)
     densitye(:,igrid(mpsi-i):igrid(mpsi-i)+mtheta(mpsi-i))=&
        densitye(:,igrid(mpsi-i):igrid(mpsi-i)+mtheta(mpsi-i))*real(i)/real(nbound)
  enddo

! flux surface average and normalization  
  zonale=0.0
  do i=0,mpsi
     do j=1,mtheta(i)
        do k=1,mzeta
           ij=igrid(i)+j
           zonale(i)=zonale(i)+densitye(k,ij)
           densitye(k,ij)=densitye(k,ij)*markere(k,ij)
        enddo        
     enddo
  enddo

! toroidal sum of phi00, broadcast to every PE
  !call MPI_ALLREDUCE(zonale,adum,mpsi+1,mpi_Rsize,MPI_SUM,toroidal_comm,ierror)
  call toroidal_allreduce_cmm(hpx4_bti,zonale,adum,mpsi+1)
  zonale=adum*pmarke

! densitye subtracted (0,0) mode
  do i=0,mpsi
     do j=1,mtheta(i)
        do k=1,mzeta
           ij=igrid(i)+j
           densitye(k,ij)=densitye(k,ij)-zonale(i)
        enddo
     enddo
! poloidal BC condition
     densitye(1:mzeta,igrid(i))=densitye(1:mzeta,igrid(i)+mtheta(i))
  enddo
  
! enforce charge conservation for zonal flow mode
  rdum=0.0
  tdum=0.0
  do i=1,mpsi-1
     r=a0+deltar*real(i)
     rdum=rdum+r
     tdum=tdum+r*zonale(i)
  enddo
  tdum=tdum/rdum
  ddene=tdum !for diagnostic
  do i=1,mpsi-1
     zonale(i)=zonale(i)-tdum
  enddo

end subroutine chargee
